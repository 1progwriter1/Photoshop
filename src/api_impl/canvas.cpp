#include <api_impl/canvas.hpp>
#include <api_canvas.hpp>
#include <cassert>
#include <iostream>


Layer::Layer( vec2u size, vec2i pos /*= vec2i()*/)
    :   size_( size), pos_( pos), pixels_( size.x * size.y, Color()) {}


Color Layer::getPixel(sfm::vec2i pos) const
{
    assert( pos.x >= 0 && size_.x > pos.x && pos.y >= 0 && size_.y > pos.y );
    return pixels_[pos.y * size_.x + pos.x];
}


void Layer::setPixel(sfm::vec2i pos, sfm::Color pixel)
{
    if ( !(pos.x >= 0 && size_.x > pos.x && pos.y >= 0 && size_.y > pos.y) )
    {
        return;
    }
    pixels_[pos.y * size_.x + pos.x] = pixel;
}


ILayer* Canvas::getLayer(size_t index)
{
    size_t cur_index = 0;
    for ( auto &layer : layers_ )
    {
        if ( cur_index == index )
        {
            return layer.get();
        }
        cur_index++;
    }
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
    return active_layer_index_;
}


void Canvas::setActiveLayerIndex(size_t index)
{
    active_layer_index_ = index;
}


bool Canvas::insertLayer(size_t index, std::unique_ptr<ILayer> layer)
{
    size_t cur_index = 0;
    auto iter = layers_.begin();
    for ( auto &cur_layer : layers_ )
    {
        if ( cur_index == index )
        {
            break;
        }
        iter++;
    }
    layers_.insert( iter, std::move( layer));

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


bool Canvas::isWindowContainer() const
{
    return false;
}


wid_t Canvas::getId() const
{
    return kCanvasWindowId;
}


vec2i Canvas::getPos() const
{
    return pos_;
}


vec2u Canvas::getSize() const
{
    return size_;
}


void Canvas::setParent(const IWindow* parent)
{
    assert( parent );
    parent_ = parent;
}

bool Canvas::isActive() const

{
    return is_active_;
}


void Canvas::forceActivate()
{
    is_active_ = true;
}


void Canvas::forceDeactivate()
{
    is_active_ = false;
}


IWindow *Canvas::getWindowById( wid_t id)
{
    return this->getId() == id ? this : nullptr;
}


const IWindow *Canvas::getWindowById( wid_t id) const
{
    return this->getId() == id ? this : nullptr;
}


void Canvas::draw( IRenderWindow *renderWindow)
{
    assert( renderWindow );

    for ( auto &layer : layers_ )
    {
        Layer *cur_layer = dynamic_cast<Layer *>( layer.get());
        assert( cur_layer );

        texture_->update( cur_layer->pixels_.data());
        sprite_->setTexture( texture_.get());
        sprite_->setPosition( pos_.x, pos_.y);
        sprite_->draw( renderWindow);
    }
}


bool Canvas::update( const IRenderWindow *renderWindow, const Event &event)
{
    if ( event.type == sfm::Event::MouseWheelScrolled )
    {
        if ( event.mouseWheel.wheel == sfm::Mouse::Wheel::Vertical )
        {
            pos_.y += event.mouseWheel.delta * 10;
        }
        else if ( event.mouseWheel.wheel == sfm::Mouse::Wheel::Horizontal )
        {
            pos_.x += event.mouseWheel.delta * 10;
        }
    }
    return true;
}


Canvas::Canvas( vec2i init_pos, vec2u init_size)
    :   is_active_( true), parent_( nullptr), size_( init_size), pos_( init_pos)
{
    texture_ = sfm::ITexture::create();
    texture_->create( init_size.x, init_size.y);

    sprite_ = sfm::ISprite::create();

    insertLayer( 0, std::make_unique<Layer>( init_size, init_pos));
    for ( int x = 0; x < init_size.x; x++ )
    {
        for ( int y = 0; y < init_size.y; y++ )
        {
            layers_.front()->setPixel( vec2i( x, y), sfm::Color( 255, 255, 255, 255));
        }
    }
    active_layer_index_ = 0;
}
