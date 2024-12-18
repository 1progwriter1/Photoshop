#include "rectangle.hpp"
#include <cassert>
#include <iostream>

#include "../headers/windows_id.hpp"
#include "../headers/api_impl/canvas.hpp"


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool onLoadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    IBar *toolbar = dynamic_cast<IBar *>( kRootWindowPtr->getWindowById( psapi::kToolBarWindowId));
    assert( toolbar && "Failed to cast to IBar" );

    std::unique_ptr<sfm::Texture> texture = std::make_unique<sfm::Texture>();
    std::unique_ptr<sfm::Sprite> sprite = std::make_unique<sfm::Sprite>();
    texture->loadFromFile("../assets/images/rectangle48_48.png");
    sprite->setTexture( texture.get());

    std::unique_ptr<ABarButton> rectangle = std::make_unique<Rectangle>( kRectangleButtonId, texture, sprite);
    rectangle->setParent( toolbar);
    toolbar->addWindow( std::move( rectangle));

    return true;
}

void onUnloadPlugin()
{

}


Rectangle::Rectangle( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite),
        canvas_( dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId))),
        options_bar_( dynamic_cast<IOptionsBar *>( getRootWindow()->getWindowById( kOptionsBarWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
    layer_ = canvas_->getTempLayer();
    assert( layer_ && "Failed to get temp layer" );
    assert( options_bar_ && "Failed to cast to options bar" );

    canvas_rect_ = psapi::getCanvasIntRect();
    canvas_rect_.pos += vec2i(0, 20);
    canvas_rect_.size -= vec2u(20, 20);
}


void Rectangle::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
    parent_->finishButtonDraw( renderWindow, this);
}


void Rectangle::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(ABarButton::createAction(renderWindow, event));
}


std::unique_ptr<IAction> Rectangle::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    if ( event.type == sfm::Event::MouseButtonReleased && draw_ )
    {
        return std::make_unique<UndoableRectangleAction>(this, renderWindow, event);
    }
    return std::make_unique<RectangleAction>(this, renderWindow, event);
}


void Rectangle::drawRectangle( const sfm::IRenderWindow *renderWindow, ILayer *layer, bool is_temp_layer)
{
    assert( renderWindow );
    assert( layer );

    sfm::vec2i mouse_pos = sfm::Mouse::getPosition( renderWindow) - canvas_rect_.pos;
    sfm::vec2u size( std::abs( mouse_pos.x - left_upper_edge_.x), std::abs( mouse_pos.y - left_upper_edge_.y));

    int edge_x = std::min( mouse_pos.x, left_upper_edge_.x);
    int edge_y = std::min( mouse_pos.y, left_upper_edge_.y);

    for ( int x = 0; x < canvas_rect_.size.x; x++ )
    {
        for ( int y = 0; y < canvas_rect_.size.y; y++ )
        {
            if ( x >= edge_x && x < edge_x + size.x && y >= edge_y && y < edge_y + size.y )
            {
                layer->setPixel( sfm::vec2i( x, y), color_);
            } else if ( is_temp_layer )
            {
                layer->setPixel( sfm::vec2i( x, y), sfm::Color(0, 0, 0, 0));
            }
        }
    }
}


bool Rectangle::isOnCanvas( sfm::vec2i mouse_pos)
{
    return canvas_rect_.pos.x <= mouse_pos.x && mouse_pos.x < canvas_rect_.pos.x + canvas_rect_.size.x &&
           canvas_rect_.pos.y <= mouse_pos.y && mouse_pos.y < canvas_rect_.pos.y + canvas_rect_.size.y;
}


void Rectangle::setColor( const sfm::Color &color)
{
    color_ = color;
}



const sfm::Color &Rectangle::getColor() const
{
    return color_;
}


void Rectangle::addOptions()
{
    for ( auto &option : options_ )
    {
        static_cast<IOptionsBar *>(psapi::getRootWindow()->getWindowById(kOptionsBarWindowId))->addWindow(std::move(option));
    }
    options_.clear();
    options_added_ = true;
}


void Rectangle::removeOptions()
{
    static_cast<IOptionsBar *>(psapi::getRootWindow()->getWindowById(kOptionsBarWindowId))->removeAllOptions();
    options_added_ = false;
}



void Rectangle::createOptions()
{
    std::unique_ptr<IColorPalette> palette = IColorPalette::create();
    palette->setColor(color_);
    palette_ = palette.get();
    options_.push_back(std::move(palette));
}


RectangleAction::RectangleAction(Rectangle *rectangle, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), rectangle_(rectangle) {}


bool RectangleAction::isUndoable(const Key &key)
{
    return false;
}


bool RectangleAction::undo(const Key &key)
{
    std::cout << "Undo rectangle\n";
    return true;
}


bool RectangleAction::redo(const Key &key)
{
    std::cout << "Redo rectangle\n";
    return true;
}


bool RectangleAction::execute(const Key &key)
{
    rectangle_->updateState(render_window_, event_);

    if ( rectangle_->getState() != IBarButton::State::Press )
    {
        rectangle_->options_added_ = false;
        return true;
    }
    if ( !rectangle_->options_added_ )
    {
        rectangle_->createOptions();
        rectangle_->addOptions();
    }
    rectangle_->color_ = rectangle_->palette_->getColor();
    static bool is_front = false;
    if ( event_.type == sfm::Event::None )
    {
        is_front = true;
    }
    if ( rectangle_->state_ != psapi::IBarButton::State::Press )
    {
        return true;
    }
    if ( event_.type == sfm::Event::MouseButtonPressed && rectangle_->isOnCanvas( sfm::Mouse::getPosition( render_window_)) && is_front )
    {
        is_front = false;
        rectangle_->draw_ = true;
        rectangle_->left_upper_edge_ = sfm::Mouse::getPosition( render_window_) - rectangle_->canvas_rect_.pos;
        rectangle_->last_mouse_pos_ = rectangle_->left_upper_edge_;
    } else if ( event_.type == sfm::Event::MouseButtonReleased && rectangle_->draw_ )
    {
        // rectangle_->canvas_->cleanTempLayer();
        // rectangle_->drawRectangle( render_window_, rectangle_->canvas_->getLayer( rectangle_->canvas_->getActiveLayerIndex()), false);
        // rectangle_->draw_ = false;
        // rectangle_->left_upper_edge_ = sfm::vec2i();
        std::cout << "Call undoable action\n";
        return true;
    }

    if ( rectangle_->draw_ && is_front )
    {
        is_front = false;
        sfm::vec2i new_mouse_pos = sfm::Mouse::getPosition( render_window_) - rectangle_->canvas_->getPos();
        rectangle_->last_mouse_pos_ = new_mouse_pos;
        rectangle_->drawRectangle( render_window_, rectangle_->layer_, true);
    }
    return true;
}


UndoableRectangleAction::UndoableRectangleAction(Rectangle *rectangle, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), rectangle_(rectangle), memento_(std::make_unique<AMementable<ILayerSnapshot>>()) {}


bool UndoableRectangleAction::isUndoable(const Key &key)
{
    return true;
}


bool UndoableRectangleAction::undo(const Key &key)
{
    std::unique_ptr<ILayerSnapshot> tmp = memento_->save();
    memento_->restore(snapshot_.get());
    snapshot_ = std::move(tmp);
    return true;
}


bool UndoableRectangleAction::redo(const Key &key)
{
    memento_->restore(snapshot_.get());
    return true;
}


bool UndoableRectangleAction::execute(const Key &key)
{
    rectangle_->updateState(render_window_, event_);
    rectangle_->canvas_->cleanTempLayer();
    snapshot_ = memento_->save();
    rectangle_->drawRectangle( render_window_, rectangle_->canvas_->getLayer( rectangle_->canvas_->getActiveLayerIndex()), false);
    rectangle_->draw_ = false;
    rectangle_->left_upper_edge_ = sfm::vec2i();

    return true;
}
