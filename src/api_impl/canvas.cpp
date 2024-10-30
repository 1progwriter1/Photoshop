#include <api_impl/canvas.hpp>
#include <api_canvas.hpp>
#include <cassert>


Layer::Layer( vec2i size, vec2i pos /*= vec2i()*/)
    :   size_( size), pos_( pos), pixels_( size.x * size.y, Color()) {}


Color Layer::getPixel(sfm::vec2i pos) const
{
    return pixels_[pos.y * size_.x + pos.x];
}


void Layer::setPixel(sfm::vec2i pos, sfm::Color pixel)
{
    pixels_[pos.y * size_.x + pos.x] = pixel;
}


ILayer* Canvas::getLayer(size_t index)
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


const ILayer* Canvas::getLayer(size_t index) const
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


ILayer* Canvas::getTempLayer()
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


const ILayer* Canvas::getTempLayer() const
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


void Canvas::cleanTempLayer()
{
    assert( 0 && "Not implemented" );
}


size_t Canvas::getNumLayers() const
{
    assert( 0 && "Not implemented" );

    return 0;
}


size_t Canvas::getActiveLayerIndex() const
{
    assert( 0 && "Not implemented" );

    return 0;
}


void Canvas::setActiveLayerIndex(size_t index)
{
    assert( 0 && "Not implemented" );
}


bool Canvas::insertLayer(size_t index, std::unique_ptr<ILayer> layer)
{
    assert( 0 && "Not implemented" );

    return true;
}


bool Canvas::removeLayer(size_t index)
{
    assert( 0 && "Not implemented" );

    return true;
}


bool Canvas::insertEmptyLayer(size_t index)
{
    assert( 0 && "Not implemented" );

    return true;
}


void Canvas::setPos(sfm::vec2i pos)
{
    assert( 0 && "Not implemented" );
}


void Canvas::setSize(sfm::vec2i size)
{
    assert( 0 && "Not implemented" );
}


void Canvas::setScale(sfm::vec2f scale)
{
    assert( 0 && "Not implemented" );
}


sfm::vec2i Canvas::getMousePosition() const
{
    assert( 0 && "Not implemented" );

    return vec2i();
}


bool Canvas::isPressed() const
{
    assert( 0 && "Not implemented" );

    return false;
}

