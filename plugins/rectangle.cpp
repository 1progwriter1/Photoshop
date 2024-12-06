#include "rectangle.hpp"
#include <cassert>
#include <iostream>

#include "../headers/windows_id.hpp"
#include "../headers/api_impl/canvas.hpp"


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool loadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    IBar *toolbar = dynamic_cast<IBar *>( kRootWindowPtr->getWindowById( psapi::kToolBarWindowId));
    assert( toolbar && "Failed to cast to IBar" );

    std::unique_ptr<sfm::Texture> texture = std::make_unique<sfm::Texture>();
    std::unique_ptr<sfm::Sprite> sprite = std::make_unique<sfm::Sprite>();
    texture->loadFromFile("../images/rectangle48_48.png");
    sprite->setTexture( texture.get());

    std::unique_ptr<ABarButton> rectangle = std::make_unique<Rectangle>( kRectangleButtonId, texture, sprite);
    rectangle->setParent( toolbar);
    toolbar->addWindow( std::move( rectangle));

    return true;
}

void unloadPlugin()
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
    return std::make_unique<RectangleAction>(this, renderWindow, event);
}


void Rectangle::drawRectangle( const sfm::IRenderWindow *renderWindow, ILayer *layer, bool is_temp_layer)
{
    assert( renderWindow );
    assert( layer );

    sfm::vec2i mouse_pos = sfm::Mouse::getPosition( renderWindow) - CANVAS_SECTOR_POS;
    sfm::vec2u size( std::abs( mouse_pos.x - left_upper_edge_.x), std::abs( mouse_pos.y - left_upper_edge_.y));
    sfm::vec2u canvas_size = canvas_->getSize();

    int edge_x = std::min( mouse_pos.x, left_upper_edge_.x);
    int edge_y = std::min( mouse_pos.y, left_upper_edge_.y);

    sfm::vec2i offset = CANVAS_SECTOR_POS - canvas_->getPos();

    for ( int x = 0; x < CANVAS_SECTOR_SIZE.x; x++ )
    {
        for ( int y = 0; y < CANVAS_SECTOR_SIZE.y; y++ )
        {
            if ( x >= edge_x && x < edge_x + size.x && y >= edge_y && y < edge_y + size.y )
            {
                layer->setPixel( sfm::vec2i( x + offset.x, y + offset.y), color_);
            } else if ( is_temp_layer )
            {
                layer->setPixel( sfm::vec2i( x + offset.x, y + offset.y), sfm::Color(0, 0, 0, 0));
            }
        }
    }
}


bool Rectangle::isOnCanvas( sfm::vec2i mouse_pos)
{
    sfm::vec2i relative_pos = mouse_pos - canvas_->getPos();
    sfm::vec2u canvas_size = canvas_->getSize();

    return 0 <= relative_pos.x && relative_pos.x < static_cast<int>( canvas_size.x) &&
           0 <= relative_pos.y && relative_pos.y < static_cast<int>( canvas_size.y);
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

}


void Rectangle::removeOptions()
{

}



void Rectangle::createOptions()
{

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
        if ( rectangle_->options_added_ )
        {
            rectangle_->removeOptions();
            rectangle_->options_added_ = false;
        }
        return true;
    }
    if ( !rectangle_->options_added_ )
    {
        rectangle_->createOptions();
        rectangle_->addOptions();
        rectangle_->options_added_ = true;
    }

    if ( rectangle_->state_ != psapi::IBarButton::State::Press )
    {
        return true;
    }
    if ( event_.type == sfm::Event::MouseButtonPressed && rectangle_->isOnCanvas( sfm::Mouse::getPosition( render_window_)) )
    {
        rectangle_->draw_ = true;
        rectangle_->left_upper_edge_ = sfm::Mouse::getPosition( render_window_) - CANVAS_SECTOR_POS;
        rectangle_->last_mouse_pos_ = rectangle_->left_upper_edge_;
    } else if ( event_.type == sfm::Event::MouseButtonReleased && rectangle_->draw_ )
    {
        rectangle_->canvas_->cleanTempLayer();
        rectangle_->drawRectangle( render_window_, rectangle_->canvas_->getLayer( rectangle_->canvas_->getActiveLayerIndex()), false);
        rectangle_->draw_ = false;
        rectangle_->left_upper_edge_ = sfm::vec2i();
        return true;
    }

    if ( rectangle_->draw_ )
    {
        sfm::vec2i new_mouse_pos = sfm::Mouse::getPosition( render_window_) - rectangle_->canvas_->getPos();
        if ( std::abs( new_mouse_pos.x - rectangle_->last_mouse_pos_.x) >= 10 || std::abs( new_mouse_pos.y - rectangle_->last_mouse_pos_.y) >= 10 )
        {
            rectangle_->last_mouse_pos_ = new_mouse_pos;
            rectangle_->drawRectangle( render_window_, rectangle_->layer_, true);
        }
    }

    return true;

    return true;
}
