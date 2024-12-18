#include "unsharp_mask.hpp"
#include <iostream>
#include <cassert>
#include "api_impl/memento.hpp"


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


sfm::Color createContrastColor( const sfm::Color &normal, const sfm::Color &blurred, int cf)
{
    int r = normal.r + (normal.r - blurred.r) * cf;
    int g = normal.g + (normal.g - blurred.g) * cf;
    int b = normal.b + (normal.b - blurred.b) * cf;

    float max_c = std::max( std::max( r, g), b);

    float r_c = r / max_c * 255.f;
    float g_c = g / max_c * 255.f;
    float b_c = b / max_c * 255.f;

    return sfm::Color( static_cast<uint8_t>( std::clamp(r, 0, 255)), static_cast<uint8_t>( std::clamp(g, 0, 255)), static_cast<uint8_t>( std::clamp(b, 0, 255)));
}


UnsharpMaskFilter::UnsharpMaskFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                               std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)), control_panel_(std::make_unique<UnsharpMaskControlPanel>()),
        memento_(std::make_unique<AMementable<ILayerSnapshot>>())
{
    control_panel_->setFilter(this);
}


std::unique_ptr<IAction> UnsharpMaskFilter::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<UnsharpMaskFilterAction>(this, renderWindow, event);
}


void UnsharpMaskFilter::draw(IRenderWindow* renderWindow)
{
    TextButton::draw(renderWindow);
    if ( control_panel_->isActive())
        control_panel_->draw(renderWindow);
}


void UnsharpMaskFilter::setOk(bool value)
{
    is_ok_ = value;
}


void UnsharpMaskFilter::setCancel(bool value)
{
    is_cancel_ = value;
}


void UnsharpMaskFilter::unsetAll()
{
    control_panel_->unsetAll();
}


void UnsharpMaskFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


void UnsharpMaskFilter::updatePanel(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(control_panel_->createAction(renderWindow, event));
}


UnsharpMaskFilterAction::UnsharpMaskFilterAction(UnsharpMaskFilter *button, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool UnsharpMaskFilterAction::isUndoable(const Key &key)
{
    return false;
}


bool UnsharpMaskFilterAction::undo(const Key &key)
{
    std::cout << "Undo contrast\n";
    return true;
}


bool UnsharpMaskFilterAction::redo(const Key &key)
{
    std::cout << "Redo contrast\n";
    return true;
}


bool UnsharpMaskFilterAction::execute(const Key &key)
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


void UnsharpMaskFilterAction::applyToTmpLayer()
{
    main_layer_ = static_cast<ILayer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( canvas_->insertEmptyLayer( 1) );
    tmp_layer_ = static_cast<ILayer *>(canvas_->getLayer( 1));

    sfm::vec2u canvas_size = getCanvasIntRect().size;
//     for ( size_t x = 0; x < canvas_size.x - 2; x++ )
//     {
//         for ( size_t y = 0; y < canvas_size.y - 2; y++ )
//         {
//             int from_x = std::max( 0, static_cast<int>( x - 2));
//             int to_x = std::min( static_cast<int>( canvas_size.x - 1), static_cast<int>( x + 2));
//
//             int from_y = std::max( 0, static_cast<int>( y - 2));
//             int to_y = std::min( static_cast<int>( canvas_size.y - 1), static_cast<int>( y + 2));
//
//             float r = 0, g = 0, b = 0, a = 0;
//
//             for ( int i = from_x; i <= to_x; i++ )
//             {
//                 for ( int j = from_y; j <= to_y; j++ )
//                 {
//                     sfm::Color color = main_layer_->getPixel( sfm::vec2i( i, j));
//                     r += color.r * GAUSS_MATRIX5_5[ (j - from_y) * 5 + (i - from_x)];
//                     g += color.g * GAUSS_MATRIX5_5[ (j - from_y) * 5 + (i - from_x)];
//                     b += color.b * GAUSS_MATRIX5_5[ (j - from_y) * 5 + (i - from_x)];
//                     a += color.a * GAUSS_MATRIX5_5[ (j - from_y) * 5 + (i - from_x)];
//                 }
//             }
//             r /= GAUSS_SUM;
//             g /= GAUSS_SUM;
//             b /= GAUSS_SUM;
//             a /= GAUSS_SUM;
//
//             sfm::Color color( static_cast<uint8_t>( r), static_cast<uint8_t>( g), static_cast<uint8_t>( b), static_cast<uint8_t>( a));
//
//             tmp_layer_->setPixel( sfm::vec2i( x, y), color);
//         }
//     }
for ( size_t x = 0; x < canvas_size.x - 2; x++ )
    {
        for ( size_t y = 0; y < canvas_size.y - 2; y++ )
        {
            int from_x = std::max( 0, static_cast<int>( x - 5));
            int to_x = std::min( static_cast<int>( canvas_size.x - 1), static_cast<int>( x + 5));
            int from_y = std::max( 0, static_cast<int>( y - 5));
            int to_y = std::min( static_cast<int>( canvas_size.y - 1), static_cast<int>( y + 5));

            int r = 0, g = 0, b = 0, a = 0;
            for ( int i = from_x; i <= to_x; i++ )
            {
                for ( int j = from_y; j <= to_y; j++ )
                {
                    sfm::Color color = main_layer_->getPixel( sfm::vec2i( i, j));
                    r += color.r;
                    g += color.g;
                    b += color.b;
                    a += color.a;
                }
            }
            r /= 121; g /= 121; b /= 121; a /= 121;
            sfm::Color color( r, g, b, a);
            tmp_layer_->setPixel( sfm::vec2i( x, y), color);
        }
    }
    button_->snapshot_ = button_->memento_->save();
    for ( int x = 0; x < canvas_size.x; x++ )
    {
        for ( int y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color normal = main_layer_->getPixel( sfm::vec2i( x, y));
            sfm::Color blurred = tmp_layer_->getPixel( sfm::vec2i( x, y));
            main_layer_->setPixel( sfm::vec2i(x, y), createContrastColor( normal, blurred, 2));
        }
    }
    canvas_->removeLayer( 1);
}


void UnsharpMaskFilterAction::applyToMainLayer()
{

}


void UnsharpMaskFilterAction::removeTmpLayer()
{
    button_->memento_->restore(button_->snapshot_.get());
}


UnsharpMaskControlPanel::UnsharpMaskControlPanel()
    :   ControlPanel()
{
    setSize(vec2u(600, 200));
    // main_shape_->setFillColor();
}


void UnsharpMaskControlPanel::setSize(const vec2u& size)
{
    ControlPanel::setSize(size);
    return;
}


void UnsharpMaskControlPanel::setPos(const vec2i& pos)
{
    ControlPanel::setPos(pos);
    return;
}


void UnsharpMaskControlPanel::draw(IRenderWindow* renderWindow)
{
    ControlPanel::draw(renderWindow);
    main_shape_->draw(renderWindow);
    line_shape_->draw(renderWindow);
    slider_shape_->draw(renderWindow);
}


void UnsharpMaskControlPanel::setFilter(UnsharpMaskFilter *parent)
{
    parent_ = parent;
}


void UnsharpMaskControlPanel::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(ControlPanel::createAction(renderWindow, event));
}


std::unique_ptr<IAction> UnsharpMaskControlPanel::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<UnsharpMaskControlPanelAction>(this, renderWindow, event);
}


UnsharpMaskControlPanelAction::UnsharpMaskControlPanelAction(UnsharpMaskControlPanel *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button) {}


bool UnsharpMaskControlPanelAction::execute(const Key &key)
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
}


bool UnsharpMaskControlPanelAction::isUndoable(const Key &key)
{
    return false;
}
