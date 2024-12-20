#include "gauss_blur.hpp"
#include "api_impl/memento.hpp"
#include <iostream>
#include <cassert>


const std::array<float, 9> GAUSS_MATRIX =
{
    0.025, 0.1, 0.025,
    0.1, 0.5, 0.1,
    0.025, 0.1, 0.025
};

const float GAUSS_SUM = 1;

const std::array<float, 25> GAUSS_MATRIX5_5 =
{
    1.f / 273,  4.f / 273,  7.f / 273,  4.f / 273, 1.f / 273,
    4.f / 273, 16.f / 273, 26.f / 273, 16.f / 273, 4.f / 273,
    7.f / 273, 26.f / 273, 41.f / 273, 26.f / 273, 4.f / 273,
    4.f / 273, 16.f / 273, 26.f / 273, 16.f / 273, 4.f / 273,
    1.f / 273,  4.f / 273,  7.f / 273,  4.f / 273, 1.f / 273
};


bool onLoadPlugin()
{
    AMenuButton *filter_bar = dynamic_cast<AMenuButton *>(getRootWindow()->getWindowById(kMenuFilterId));
    assert( filter_bar && "Failed to cast to menu button" );

    std::unique_ptr<sfm::IFont> font = sfm::IFont::create();
    font->loadFromFile("../assets/fonts/Normal.ttf");
    std::unique_ptr<sfm::IText> button_text = sfm::IText::create();

    button_text->setFont(font.get());
    sfm::Color color = sfm::Color::getStandardColor(psapi::sfm::Color::Type::White);
    button_text->setFillColor(&color);
    button_text->setString("Gauss Blur");

    std::unique_ptr<sfm::IRectangleShape> shape = sfm::RectangleShape::create(10, 10); // default size, it is not used later
    shape->setFillColor(sfm::Color::getStandardColor(psapi::sfm::Color::Type::Transparent));

    std::unique_ptr<GaussBlurFilter> filter = std::make_unique<GaussBlurFilter>(1234, std::move(font),
                                                                std::move(button_text),
                                                                std::move(shape));

    filter_bar->addMenuItem(std::move(filter));

    return true;
}


void onUnloadPlugin()
{

}


GaussBlurFilter::GaussBlurFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                                 std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)), control_panel_(std::make_unique<GaussBlurControlPanel>()),
        memento_(std::make_unique<AMementable<ILayerSnapshot>>())
{
    control_panel_->setFilter(this);
}


std::unique_ptr<IAction> GaussBlurFilter::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<GaussBlurFilterAction>(this, renderWindow, event);
}


void GaussBlurFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


void GaussBlurFilter::unsetAll()
{
    control_panel_->unsetAll();
}


void GaussBlurFilter::setOk(bool value)
{
    is_ok_ = value;
}


void GaussBlurFilter::setCancel(bool value)
{
    is_cancel_ = value;
}


void GaussBlurFilter::updatePanel(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(control_panel_->createAction(renderWindow, event));
}


GaussBlurFilterAction::GaussBlurFilterAction(GaussBlurFilter* filter, const IRenderWindow* renderWindow, const Event& event)
    :   AUndoableAction(renderWindow, event), button_(filter)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool GaussBlurFilterAction::execute(const Key& key)
{
    button_->updateState(render_window_, event_);
    if ( button_->getState() != IBarButton::State::Press )
    {
        if ( button_->getState() != psapi::IBarButton::State::Hover )
        {
            button_->control_panel_->forceDeactivate();
        }
        return true;
    }
    button_->control_panel_->forceActivate();
    button_->updatePanel(render_window_, event_);
    if ( !button_->is_applied_ )
    {
        applyToTmpLayer();
        button_->is_applied_ = true;
    }
    if ( button_->is_ok_)
    {
        applyToMainLayer();
    }
    else if ( button_->is_cancel_ )
    {
        removeTmpLayer();
    } else
    {
        return true;
    }
    button_->unsetAll();
    button_->setState(psapi::IBarButton::State::Normal);
    button_->is_applied_ = false;
    button_->is_ok_ = false;
    button_->is_cancel_ = false;

    return true;
}


void GaussBlurFilterAction::applyToTmpLayer()
{
    main_layer_ = static_cast<ILayer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( canvas_->insertEmptyLayer( 1) );

    button_->snapshot_ = button_->memento_->save();

    sfm::vec2u canvas_size = getCanvasIntRect().size;
    for ( size_t x = 0; x < canvas_size.x - 2; x++ )
    {
        for ( size_t y = 0; y < canvas_size.y - 2; y++ )
        {
            int from_x = std::max( 0, static_cast<int>( x - 1));
            int to_x = std::min( static_cast<int>( canvas_size.x - 1), static_cast<int>( x + 1));
            int from_y = std::max( 0, static_cast<int>( y - 1));
            int to_y = std::min( static_cast<int>( canvas_size.y - 1), static_cast<int>( y + 1));

            float r = 0, g = 0, b = 0, a = 0;
            for ( int i = from_x; i <= to_x; i++ )
            {
                for ( int j = from_y; j <= to_y; j++ )
                {
                    sfm::Color color = main_layer_->getPixel( sfm::vec2i( i, j));
                    r += color.r * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    g += color.g * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    b += color.b * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    a += color.a * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                }
            }
            r /= GAUSS_SUM; g /= GAUSS_SUM; b /= GAUSS_SUM; a /= GAUSS_SUM;
            sfm::Color color( static_cast<uint8_t>( r), static_cast<uint8_t>( g), static_cast<uint8_t>( b), static_cast<uint8_t>( a));
            main_layer_->setPixel( sfm::vec2i( x, y), color);
        }
    }
}


void GaussBlurFilterAction::applyToMainLayer()
{

}


void GaussBlurFilterAction::removeTmpLayer()
{
    button_->memento_->restore(button_->snapshot_.get());
}



bool GaussBlurFilterAction::undo(const Key& key)
{
    std::cout << "Undo gauss blur\n";
    return true;
}


bool GaussBlurFilterAction::redo(const Key& key)
{
    std::cout << "Redo gauss blur\n";
    return true;
}


bool GaussBlurFilterAction::isUndoable(const Key& key)
{
    return false;
}


GaussBlurControlPanel::GaussBlurControlPanel()
{
    setSize(vec2u(100, 32));
}


std::unique_ptr<IAction> GaussBlurControlPanel::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<GaussBlurControlPanelAction>(this, renderWindow, event);
}


void GaussBlurFilter::draw(IRenderWindow* renderWindow)
{
    TextButton::draw(renderWindow);
    if ( control_panel_->isActive())
        control_panel_->draw(renderWindow);
}


void GaussBlurControlPanel::draw(IRenderWindow* renderWindow)
{
    ControlPanel::draw(renderWindow);
}


void GaussBlurControlPanel::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(ControlPanel::createAction(renderWindow, event));
}


void GaussBlurControlPanel::setFilter(GaussBlurFilter *parent)
{
    parent_ = parent;
}


GaussBlurControlPanelAction::GaussBlurControlPanelAction(GaussBlurControlPanel *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button) {}


bool GaussBlurControlPanelAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);
    if ( button_->getOkState() == IBarButton::State::Press )
    {
        button_->parent_->setOk(true);
        button_->parent_->setCancel(false);
    } else if ( button_->getCancelState() == IBarButton::State::Press )
    {
        button_->parent_->setCancel(true);
        button_->parent_->setOk(false);
    }
    return true;
    return true;
}


bool GaussBlurControlPanelAction::isUndoable(const Key &key)
{
    return false;
}
