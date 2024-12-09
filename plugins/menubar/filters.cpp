#include "filters.hpp"
#include <cassert>
#include <iostream>
#include "api_impl/canvas.hpp"


const std::array<float, 9> GAUSS_MATRIX =
{
    0.025, 0.1, 0.025,
    0.1, 0.5, 0.1,
    0.025, 0.1, 0.025
};
const float GAUSS_SUM = 1;


BareliefFilter::BareliefFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                               std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


std::unique_ptr<IAction> BareliefFilter::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<BareliefFilterAction>(this, renderWindow, event);
}


void BareliefFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


BareliefFilterAction::BareliefFilterAction(BareliefFilter *button, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool BareliefFilterAction::isUndoable(const Key &key)
{
    return false;
}


bool BareliefFilterAction::undo(const Key &key)
{
    std::cout << "Undo filter\n";
    return true;
}


bool BareliefFilterAction::redo(const Key &key)
{
    std::cout << "Redo filter\n";
    return true;
}


bool BareliefFilterAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);

    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }

    Layer *layer = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( layer );
    canvas_->insertEmptyLayer( 1);
    Layer *new_layer = static_cast<Layer *>(canvas_->getLayer( 1));
    assert( new_layer );
    assert( new_layer != layer );

    sfm::vec2u canvas_size = canvas_->getSize();
    for ( unsigned x = 0; x < canvas_size.x; x++ )
    {
        for ( unsigned y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color color_1 = layer->getPixelGlobal( sfm::vec2i( std::min( x + 4, canvas_size.x - 1), y));
            sfm::Color color_2 = layer->getPixelGlobal( sfm::vec2i( x, std::min( y + 4, canvas_size.y - 1)));

            sfm::Color color;
            color.r = (255 - color_1.r + color_2.r) / 2;
            color.g = (255 - color_1.g + color_2.g) / 2;
            color.b = (255 - color_1.b + color_2.b) / 2;
            color.a = (255 - color_1.a + color_2.a) / 2;
            new_layer->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }

    canvas_->removeLayer( 0);
    button_->setState( IBarButton::State::Normal);

    return true;
}


ContrastFilter::ContrastFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                               std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


std::unique_ptr<IAction> ContrastFilter::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<ContrastFilterAction>(this, renderWindow, event);
}


void ContrastFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
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
        return true;
    }

    Layer *layer = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( layer );
    assert( canvas_->insertEmptyLayer( 1) );
    Layer *new_layer = static_cast<Layer *>(canvas_->getLayer( 1));
    assert( new_layer );
    assert( new_layer != layer );

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
            r /= GAUSS_SUM;
            g /= GAUSS_SUM;
            b /= GAUSS_SUM;
            a /= GAUSS_SUM;

            sfm::Color color( static_cast<uint8_t>( r), static_cast<uint8_t>( g), static_cast<uint8_t>( b), static_cast<uint8_t>( a));

            new_layer->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }
    for ( int x = 0; x < canvas_size.x; x++ )
    {
        for ( int y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color normal = layer->getPixelGlobal( sfm::vec2i( x, y));
            sfm::Color blurred = new_layer->getPixelGlobal( sfm::vec2i( x, y));
            new_layer->setPixelGlobal( sfm::vec2i(x, y), createContrastColor( normal, blurred, 2));
        }
    }
    canvas_->removeLayer( 0);
    button_->setState( IBarButton::State::Normal);

    return true;
}


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


NegativeFilter::NegativeFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                               std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


void NegativeFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


std::unique_ptr<IAction> NegativeFilter::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<NegativeFilterAction>(this, renderWindow, event);
}


NegativeFilterAction::NegativeFilterAction(NegativeFilter *button, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool NegativeFilterAction::execute(const Key& key)
{
    button_->updateState(render_window_, event_);
    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }

    Layer *layer = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( layer );

    sfm::vec2u canvas_size = canvas_->getSize();
    for ( size_t x = 0; x < canvas_size.x; x++ )
    {
        for ( size_t y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color color = layer->getPixelGlobal( sfm::vec2i( x, y));
            color.r = 255 - color.r;
            color.g = 255 - color.g;
            color.b = 255 - color.b;
            layer->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }
    button_->setState( IBarButton::State::Normal);

    return true;
}


bool NegativeFilterAction::undo(const Key& key)
{
    std::cout << "Undo negative filter\n";
    return true;
}


bool NegativeFilterAction::redo(const Key& key)
{
    std::cout << "Redo negative filter\n";
    return true;
}


bool NegativeFilterAction::isUndoable(const Key& key)
{
    return false;
}


BrightnessFilter::BrightnessFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                                   std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


void BrightnessFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


std::unique_ptr<IAction> BrightnessFilter::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<BrightnessFilterAction>(this, renderWindow, event);
}


BrightnessFilterAction::BrightnessFilterAction(BrightnessFilter *button, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool BrightnessFilterAction::execute(const Key& key)
{
    button_->updateState(render_window_, event_);
    int delta = -10;
    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }

    Layer *layer = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( layer );

    sfm::vec2u canvas_size = canvas_->getSize();
    for ( size_t x = 0; x < canvas_size.x; x++ )
    {
        for ( size_t y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color color = layer->getPixelGlobal( sfm::vec2i( x, y));
            color.a += delta;
            layer->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }
    button_->setState( IBarButton::State::Normal);

    return true;
}


bool BrightnessFilterAction::undo(const Key& key)
{
    std::cout << "Undo brightness filter\n";
    return true;
}


bool BrightnessFilterAction::redo(const Key& key)
{
    std::cout << "Redo brightness filter\n";
    return true;
}


bool BrightnessFilterAction::isUndoable(const Key& key)
{
    return false;
}
