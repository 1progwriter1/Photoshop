#include <api_impl/canvas.hpp>
#include <api_canvas.hpp>
#include <cassert>
#include <iostream>


Layer::Layer( vec2u size, vec2i pos /*= vec2i()*/)
    :   size_( size), pos_( pos), pixels_( size.x * size.y, Color(0, 0, 0, 0)) {}


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


vec2u Layer::getSize() const
{
    return size_;
}


drawable_id_t Layer::addDrawable(std::unique_ptr<sfm::Drawable> object)
{
    assert( 0 && "Not implemented" );

    drawables_.emplace_back(std::move(object));

    return drawables_.size() - 1;
}



void Layer::removeDrawable(drawable_id_t id)
{
    assert( 0 && "Not implemented" );

    auto iter = drawables_.begin();
    std::advance(iter, id);

    drawables_.erase(iter);
}


void Layer::removeAllDrawables()
{
    drawables_.clear();
}


std::unique_ptr<ILayerSnapshot> Layer::save()
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


void Layer::restore(ILayerSnapshot *snapshot)
{
    assert( 0 && "Not implemented" );
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
    if ( index >= layers_.size() )
        return nullptr;

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


ILayer* Canvas::getTempLayer()
{
    return temp_layer_.get();
}


const ILayer* Canvas::getTempLayer() const
{
    return temp_layer_.get();
}


void Canvas::cleanTempLayer()
{
    for ( int x = 0; x < size_.x; x++ )
    {
        for ( int y = 0; y < size_.y; y++ )
        {
            temp_layer_->setPixel( vec2i( x, y), sfm::Color(0, 0, 0, 0));
        }
    }
}


size_t Canvas::getNumLayers() const
{
    return layers_.size();
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
    if ( getNumLayers() <= index )
        return false;

    auto iter = layers_.begin();
    std::advance( iter, index);
    layers_.erase( iter);

    return true;
}


bool Canvas::insertEmptyLayer(size_t index)
{
    if ( layers_.size() < index )
        return false;

    std::unique_ptr<ILayer> new_layer = std::make_unique<Layer>( size_, pos_);
    for ( int x = 0; x < size_.x; x++ )
    {
        for ( int y = 0; y < size_.y; y++ )
        {
            new_layer->setPixel( vec2i( x, y), sfm::Color(0, 0, 0, 0));
        }
    }

    auto iter = layers_.begin();
    std::advance( iter, index);
    layers_.insert( iter, std::move( new_layer));

    return true;
}


void Canvas::setPos(const sfm::vec2i &pos)
{
    pos_ = pos;
}


void Canvas::setSize(const sfm::vec2u &size)
{
    assert( size.x >= 0 && size.y >= 0 );

    size_ = size;
}


void Canvas::setZoom(sfm::vec2f zoom)
{
    scale_ = zoom;
}


Color Canvas::getCanvasBaseColor() const
{
    return Color();
}


sfm::vec2i Canvas::getMousePosition() const
{
    vec2i mouse_pos = sfm::Mouse::getPosition();

    return mouse_pos;
}


bool Canvas::isPressedLeftMouseButton() const
{
    assert( 0 && "Not implemented" );

    return false;
}


bool Canvas::isPressedRightMouseButton() const
{
    assert( 0 && "Not implemented" );

    return false;
}


bool Canvas::isPressedScrollButton() const
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

    vec2i offset = CANVAS_SECTOR_POS - pos_;
    sprite_->setTextureRect( sfm::IntRect(offset, CANVAS_SECTOR_SIZE));
    sprite_->setPosition( CANVAS_SECTOR_POS.x, CANVAS_SECTOR_POS.y);

    for ( auto &layer : layers_ )
    {
        Layer *cur_layer = static_cast<Layer *>( layer.get());

        texture_->update( cur_layer->pixels_.data());
        sprite_->setTexture( texture_.get());
        sprite_->draw( renderWindow);
    }

    Layer *cur_layer = static_cast<Layer *>( temp_layer_.get());

    texture_->update( cur_layer->pixels_.data());
    sprite_->setTexture( texture_.get());
    sprite_->draw( renderWindow);

    VerticalScroll::draw( renderWindow);
    HorizontalScroll::draw( renderWindow);
}


Canvas::Canvas( vec2i init_pos, vec2u init_size)
    :   VerticalScroll( CANVAS_SECTOR_POS, CANVAS_SECTOR_SIZE, init_pos, init_size),
        HorizontalScroll( CANVAS_SECTOR_POS, CANVAS_SECTOR_SIZE, init_pos, init_size),
        is_active_( true), parent_( nullptr), size_( init_size), pos_( init_pos), temp_layer_( std::make_unique<Layer>( init_size, init_pos))
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
            temp_layer_->setPixel( vec2i( x, y), sfm::Color(0, 0, 0, 0));
        }
    }
    active_layer_index_ = 0;
}


std::unique_ptr<IAction> Canvas::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


std::unique_ptr<ICanvasSnapshot> Canvas::save()
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


void Canvas::restore(ICanvasSnapshot* snapshot)
{
    assert( 0 && "Not implemented" );
}
