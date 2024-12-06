#include <api_impl/canvas.hpp>
#include <api_canvas.hpp>
#include <cassert>
#include <iostream>


Layer::Layer(Canvas *canvas)
    :   canvas_(canvas)
{
    assert( canvas_ );
    rect_ = canvas_->getActualRect();
    vec2u size = canvas_->getSize();
    pixels_.resize( size.x * size.y, Color(0, 0, 0, 0));
}


Color Layer::getPixel(sfm::vec2i pos) const
{
    if  ( !(pos.x >= rect_.pos.x && rect_.size.x + rect_.pos.x > pos.x &&
            pos.y >= rect_.pos.y && rect_.size.y + rect_.pos.x > pos.y ) )
        return Color();
    return pixels_[pos.y * canvas_->getSize().x + pos.x];
}


void Layer::setPixel(sfm::vec2i pos, sfm::Color pixel)
{
    pos -= rect_.pos - canvas_->getActualRect().pos;
    if  ( !(pos.x >= 0 && rect_.size.x > pos.x &&
            pos.y >= 0 && rect_.size.y > pos.y ) )
        return;

    pos += rect_.pos - canvas_->getPos();
    pixels_[pos.y * canvas_->getSize().x + pos.x] = pixel;
}


vec2u Layer::getSize() const
{
    return canvas_->getSize();
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
    Layer *layer = static_cast<Layer *>(temp_layer_.get());
    for ( int x = 0; x < size_.x; x++ )
    {
        for ( int y = 0; y < size_.y; y++ )
        {
            layer->pixels_[y * size_.x + x] = sfm::Color();
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
    if ( layer->getSize().x != size_.x || layer->getSize().y != size_.y )
        return false;

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

    std::unique_ptr<Layer> new_layer = std::make_unique<Layer>(this);
    for ( int x = 0; x < size_.x; x++ )
    {
        for ( int y = 0; y < size_.y; y++ )
        {
            new_layer->pixels_[y * size_.x + x] = sfm::Color();
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
    layers_.begin();
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

    for ( auto &layer : layers_ )
    {
        Layer *cur_layer = static_cast<Layer *>( layer.get());

        texture_->update( cur_layer->pixels_.data());
        sprite_->setTexture( texture_.get());
        sprite_->setPosition(pos_.x, pos_.y);
        sprite_->draw( renderWindow);
    }

    Layer *cur_layer = static_cast<Layer *>( temp_layer_.get());

    texture_->update( cur_layer->pixels_.data());
    sprite_->setTexture( texture_.get());
    sprite_->draw( renderWindow);
    sprite_->setPosition(pos_.x, pos_.y);

    v_scroll_.draw( renderWindow);
    h_scroll_.draw( renderWindow);
}


Canvas::Canvas( vec2i init_pos, vec2u init_size)
    :   is_active_( true), parent_( nullptr), size_( init_size - vec2u(20, 20)), pos_( init_pos + sfm::vec2i(0, 20)), temp_layer_( nullptr),
        v_scroll_( psapi::getCanvasIntRect().pos, psapi::getCanvasIntRect().size, pos_, size_),
        h_scroll_( psapi::getCanvasIntRect().pos, psapi::getCanvasIntRect().size, pos_, size_)
{
    texture_ = sfm::ITexture::create();
    texture_->create( size_.x, size_.y);

    actual_rect_ = psapi::getCanvasIntRect();
    actual_rect_.pos += vec2i(0, 20);
    actual_rect_.size -= vec2u(20, 20);

    sprite_ = sfm::ISprite::create();
    sprite_->setPosition(static_cast<float>(pos_.x), static_cast<float>(pos_.y));

    std::unique_ptr<Layer> layer = std::make_unique<Layer>( this);
    std::unique_ptr<Layer> temp_layer = std::make_unique<Layer>( this);
    for ( int x = 0; x < size_.x; x++ )
    {
        for ( int y = 0; y < size_.y; y++ )
        {
            layer->pixels_[y * size_.x + x] = sfm::Color::getStandardColor(psapi::sfm::Color::Type::White);
            temp_layer->pixels_[y * size_.x + x] = sfm::Color::getStandardColor(sfm::Color::Type::Transparent);
        }
    }
    insertLayer( 0, std::move(layer));
    temp_layer_ = std::move(temp_layer);
    active_layer_index_ = 0;
}


std::unique_ptr<IAction> Canvas::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<CanvasAction>(this, renderWindow, event);
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


CanvasAction::CanvasAction(Canvas* canvas, const IRenderWindow* renderWindow, const Event& event)
    :   AAction(renderWindow, event) ,canvas_(canvas) {}


bool CanvasAction::isUndoable(const Key &key)
{
    return false;
}


bool CanvasAction::execute(const Key &key)
{
    canvas_->h_scroll_.update(render_window_, event_, canvas_->pos_);
    canvas_->v_scroll_.update(render_window_, event_, canvas_->pos_);

    return true;
}


sfm::IntRect Canvas::getActualRect() const
{
    return actual_rect_;
}

