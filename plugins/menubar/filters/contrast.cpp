#include "contrast.hpp"


sfm::Color createContrastColor( const sfm::Color &normal, const sfm::Color &blurred, int cf)
{
    float r = normal.r / 255.f + (normal.r - blurred.r) * cf / 255.f;
    float g = normal.g / 255.f + (normal.g - blurred.g) * cf / 255.f;
    float b = normal.b / 255.f + (normal.b - blurred.b) * cf / 255.f;

    float max_c = std::max( std::max( r, g), b);

    float r_c = r / max_c * 255.f;
    float g_c = g / max_c * 255.f;
    float b_c = b / max_c * 255.f;

    return sfm::Color( static_cast<uint8_t>( r_c), static_cast<uint8_t>( g_c), static_cast<uint8_t>( b_c));
}


ContrastFilter::ContrastFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                               std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)), control_panel_(std::make_unique<ContrastControlPanel>())
{
    control_panel_->setFilter(this);
}


std::unique_ptr<IAction> ContrastFilter::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<ContrastFilterAction>(this, renderWindow, event);
}


void ContrastFilter::draw(IRenderWindow* renderWindow)
{
    TextButton::draw(renderWindow);
    if ( control_panel_->isActive())
        control_panel_->draw(renderWindow);
}


void ContrastFilter::setOk(bool value)
{
    is_ok_ = value;
}


void ContrastFilter::setCancel(bool value)
{
    is_cancel_ = value;
}


void ContrastFilter::unsetAll()
{
    control_panel_->unsetAll();
}


void ContrastFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


void ContrastFilter::updatePanel(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(control_panel_->createAction(renderWindow, event));
}


ContrastFilterAction::ContrastFilterAction(ContrastFilter *button, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool ContrastFilterAction::isUndoable(const Key &key)
{
    return false;
}


bool ContrastFilterAction::undo(const Key &key)
{
    std::cout << "Undo contrast\n";
    return true;
}


bool ContrastFilterAction::redo(const Key &key)
{
    std::cout << "Redo contrast\n";
    return true;
}


bool ContrastFilterAction::execute(const Key &key)
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


void ContrastFilterAction::applyToTmpLayer()
{
    main_layer_ = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( canvas_->insertEmptyLayer( 1) );
    tmp_layer_ = static_cast<Layer *>(canvas_->getLayer( 1));

    sfm::vec2u canvas_size = canvas_->getSize();

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
                    sfm::Color color = main_layer_->getPixelGlobal( sfm::vec2i( i, j));
                    r += color.r * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    g += color.g * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    b += color.b * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    a += color.a * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                }
            }
            r /= GAUSS_SUM;
            g /= GAUSS_SUM;
            b /= GAUSS_SUM;
            a /= GAUSS_SUM;

            sfm::Color color( static_cast<uint8_t>( r), static_cast<uint8_t>( g), static_cast<uint8_t>( b), static_cast<uint8_t>( a));

            tmp_layer_->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }
    for ( int x = 0; x < canvas_size.x; x++ )
    {
        for ( int y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color normal = main_layer_->getPixelGlobal( sfm::vec2i( x, y));
            sfm::Color blurred = tmp_layer_->getPixelGlobal( sfm::vec2i( x, y));
            tmp_layer_->setPixelGlobal( sfm::vec2i(x, y), createContrastColor( normal, blurred, 1));
        }
    }
}


void ContrastFilterAction::applyToMainLayer()
{
    canvas_->removeLayer(canvas_->getActiveLayerIndex());
    canvas_->setActiveLayerIndex(canvas_->getActiveLayerIndex());
}


void ContrastFilterAction::removeTmpLayer()
{
    canvas_->removeLayer(1);
}
