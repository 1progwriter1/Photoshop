#include "blur.hpp"
#include <cassert>
#include <iostream>


BlurFilter::BlurFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
            std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


std::unique_ptr<IAction> BlurFilter::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<BlurFilterAction>(this, renderWindow, event);
}


void BlurFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


BlurFilterAction::BlurFilterAction(BlurFilter* filter, const IRenderWindow* renderWindow, const Event& event)
    :   AUndoableAction(renderWindow, event), button_(filter)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool BlurFilterAction::execute(const Key& key)
{
    button_->updateState(render_window_, event_);

    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }
    Layer *layer = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( layer );

    sfm::vec2u canvas_size = canvas_->getSize();
    for ( size_t x = 0; x < canvas_size.x - 2; x++ )
    {
        for ( size_t y = 0; y < canvas_size.y - 2; y++ )
        {
            int from_x = std::max( 0, static_cast<int>( x - 1));
            int to_x = std::min( static_cast<int>( canvas_size.x - 1), static_cast<int>( x + 1));
            int from_y = std::max( 0, static_cast<int>( y - 1));
            int to_y = std::min( static_cast<int>( canvas_size.y - 1), static_cast<int>( y + 1));

            int r = 0, g = 0, b = 0, a = 0;
            for ( int i = from_x; i <= to_x; i++ )
            {
                for ( int j = from_y; j <= to_y; j++ )
                {
                    sfm::Color color = layer->getPixelGlobal( sfm::vec2i( i, j));
                    r += color.r;
                    g += color.g;
                    b += color.b;
                    a += color.a;
                }
            }
            r /= 9; g /= 9; b /= 9; a /= 9;
            sfm::Color color( r, g, b, a);
            layer->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }
    button_->setState( IBarButton::State::Normal);

    return true;
}


bool BlurFilterAction::undo(const Key& key)
{
    std::cout << "Undo blur\n";

    return true;
}


bool BlurFilterAction::redo(const Key& key)
{
    std::cout << "Redo blur\n";
    return true;
}


bool BlurFilterAction::isUndoable(const Key& key)
{
    return false;
}
