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
    }
    if ( draw_ )
    {
        if ( !layer_ )
        {
            canvas_->cleanTempLayer();
            layer_ = canvas_->getTempLayer();
        }
        if ( event.type == sfm::Event::MouseButtonReleased && draw_ )
        {
            drawFinalVersion( renderWindow);
            draw_ = false;
            left_upper_edge_ = sfm::vec2i();
            return true;
        } else
        {
            drawTempVersion( renderWindow);
        }
    }

    return true;
}


void Rectangle::drawTempVersion( const sfm::IRenderWindow *renderWindow)
{
    assert( renderWindow );

    sfm::vec2i mouse_pos = sfm::Mouse::getPosition( renderWindow) - canvas_->getPos();
    sfm::vec2u size( std::abs( mouse_pos.x - left_upper_edge_.x), std::abs( mouse_pos.y - left_upper_edge_.y));

    int edge_x = std::min( mouse_pos.x, left_upper_edge_.x);
    int edge_y = std::min( mouse_pos.y, left_upper_edge_.y);

    sfm::Color rect_color( 0, 150, 0);
    canvas_->cleanTempLayer();

    for ( int x = 0; x < size.x; x++ )
    {
        for ( int y = 0; y < size.y; y++ )
        {
            layer_->setPixel( sfm::vec2i( x + edge_x, y + edge_y), rect_color);
        }
    }
}


void Rectangle::drawFinalVersion( const sfm::IRenderWindow *renderWindow)
{
    assert( renderWindow );

    sfm::vec2i mouse_pos = sfm::Mouse::getPosition( renderWindow) - canvas_->getPos();
    sfm::vec2u size( std::abs( mouse_pos.x - left_upper_edge_.x), std::abs( mouse_pos.y - left_upper_edge_.y));

    int offset_x = std::min( mouse_pos.x, left_upper_edge_.x);
    int offset_y = std::min( mouse_pos.y, left_upper_edge_.y);

    sfm::Color rect_color( 0, 150, 0);
    canvas_->cleanTempLayer();
    ILayer *layer = canvas_->getLayer( canvas_->getActiveLayerIndex());
    assert( layer );

    for ( int x = 0; x < size.x; x++ )
    {
        for ( int y = 0; y < size.y; y++ )
        {
            layer->setPixel( sfm::vec2i( x + offset_x, y + offset_y), rect_color);
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
