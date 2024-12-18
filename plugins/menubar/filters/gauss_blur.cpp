#include "gauss_blur.hpp"
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


GaussBlurFilter::GaussBlurFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                                 std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


std::unique_ptr<IAction> GaussBlurFilter::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<GaussBlurFilterAction>(this, renderWindow, event);
}


void GaussBlurFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
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
        return true;
    }

    Layer *layer = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( layer );
    assert( canvas_->insertEmptyLayer( 1) );
    Layer *new_layer = static_cast<Layer *>(canvas_->getLayer( 1));
    assert( new_layer );

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
                    sfm::Color color = layer->getPixelGlobal( sfm::vec2i( i, j));
                    r += color.r * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    g += color.g * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    b += color.b * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                    a += color.a * GAUSS_MATRIX[ (j - from_y) * 3 + (i - from_x)];
                }
            }
            r /= GAUSS_SUM; g /= GAUSS_SUM; b /= GAUSS_SUM; a /= GAUSS_SUM;
            sfm::Color color( static_cast<uint8_t>( r), static_cast<uint8_t>( g), static_cast<uint8_t>( b), static_cast<uint8_t>( a));
            new_layer->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }
    canvas_->removeLayer( 0);
    button_->setState( IBarButton::State::Normal);

    return true;
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
