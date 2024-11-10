#include "rectangle.hpp"
#include <cassert>
#include <iostream>


psapi::IWindowContainer *kRootWindowPtr = nullptr;
const wid_t kRectangleButtonId = 3;


bool loadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    psapi::IBar *toolbar = dynamic_cast<psapi::IBar *>( kRootWindowPtr->getWindowById( psapi::kToolBarWindowId));
    assert( toolbar && "Failed to cast to IBar" );

    std::unique_ptr<sfm::Texture> texture = std::make_unique<sfm::Texture>();
    std::unique_ptr<sfm::Sprite> sprite = std::make_unique<sfm::Sprite>();
    texture->loadFromFile("../images/rectangle48_48.png");
    sprite->setTexture( texture.get());
    sprite->setPosition( 36, 228);

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
        canvas_( dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
    layer_ = canvas_->getTempLayer();
    assert( layer_ && "Failed to get temp layer" );
}


void Rectangle::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
    parent_->finishButtonDraw( renderWindow, this);
}


bool Rectangle::update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event)
{
    ABarButton::update( renderWindow, event);

    if ( state_ != psapi::IBarButton::State::Press )
    {
        return true;
    }
    if ( event.type == sfm::Event::MouseButtonPressed && isOnCanvas( sfm::Mouse::getPosition( renderWindow)) )
    {
        draw_ = true;
        left_upper_edge_ = sfm::Mouse::getPosition( renderWindow) - canvas_->getPos();
        last_mouse_pos_ = left_upper_edge_;
    } else if ( event.type == sfm::Event::MouseButtonReleased && draw_ )
    {
        canvas_->cleanTempLayer();
        drawRectangle( renderWindow, canvas_->getLayer( canvas_->getActiveLayerIndex()), false);
        draw_ = false;
        left_upper_edge_ = sfm::vec2i();
        return true;
    }

    if ( draw_ )
    {
        sfm::vec2i new_mouse_pos = sfm::Mouse::getPosition( renderWindow) - canvas_->getPos();
        if ( std::abs( new_mouse_pos.x - last_mouse_pos_.x) >= 10 || std::abs( new_mouse_pos.y - last_mouse_pos_.y) >= 10 )
        {
            last_mouse_pos_ = new_mouse_pos;
            drawRectangle( renderWindow, layer_, true);
        }
    }

    return true;
}


void Rectangle::drawRectangle( const sfm::IRenderWindow *renderWindow, ILayer *layer, bool is_temp_layer)
{
    assert( renderWindow );
    assert( layer );

    sfm::vec2i mouse_pos = sfm::Mouse::getPosition( renderWindow) - canvas_->getPos();
    sfm::vec2u size( std::abs( mouse_pos.x - left_upper_edge_.x), std::abs( mouse_pos.y - left_upper_edge_.y));
    sfm::vec2u canvas_size = canvas_->getSize();

    int edge_x = std::min( mouse_pos.x, left_upper_edge_.x);
    int edge_y = std::min( mouse_pos.y, left_upper_edge_.y);

    sfm::Color rect_color( 0, 150, 0);
    for ( int x = 0; x < canvas_size.x; x++ )
    {
        for ( int y = 0; y < canvas_size.y; y++ )
        {
            if ( x >= edge_x && x < edge_x + size.x && y >= edge_y && y < edge_y + size.y )
            {
                layer->setPixel( sfm::vec2i( x, y), rect_color);
            } else if ( is_temp_layer )
            {
                layer->setPixel( sfm::vec2i( x, y), sfm::Color(0, 0, 0, 0));
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
