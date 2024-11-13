#include "line.hpp"
#include <cassert>
#include "../headers/windows_id.hpp"


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool loadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    psapi::IBar *toolbar = dynamic_cast<psapi::IBar *>( kRootWindowPtr->getWindowById( psapi::kToolBarWindowId));
    assert( toolbar && "Failed to cast to IBar" );

    std::unique_ptr<sfm::Texture> texture = std::make_unique<sfm::Texture>();
    std::unique_ptr<sfm::Sprite> sprite = std::make_unique<sfm::Sprite>();
    texture->loadFromFile("../images/line48_48.png");
    sprite->setTexture( texture.get());
    sprite->setPosition( 36, 420);

    std::unique_ptr<ABarButton> line = std::make_unique<Line>( kLineButtonId, texture, sprite);
    line->setParent( toolbar);
    toolbar->addWindow( std::move( line));

    return true;
}

void unloadPlugin()
{

}


Line::Line( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite), canvas_( dynamic_cast<ICanvas *>( kRootWindowPtr->getWindowById( psapi::kCanvasWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
    layer_ = canvas_->getTempLayer();
    assert( layer_ && "Failed to get temp layer" );
}


void Line::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
    parent_->finishButtonDraw( renderWindow, this);
}


bool Line::update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event)
{
    ABarButton::update( renderWindow, event);

    if ( state_ != psapi::IBarButton::State::Press )
    {
        return true;
    }
    if ( event.type == sfm::Event::MouseButtonPressed && isOnCanvas( sfm::Mouse::getPosition( renderWindow)) )
    {
        draw_ = true;
        begin_pos_ = sfm::Mouse::getPosition( renderWindow) - CANVAS_SECTOR_POS;
        last_mouse_pos_ = begin_pos_;
    } else if ( event.type == sfm::Event::MouseButtonReleased && draw_ )
    {
        drawLine( renderWindow, canvas_->getLayer( canvas_->getActiveLayerIndex()), false);
        draw_ = false;
        begin_pos_ = sfm::vec2i();
        return true;
    }

    if ( draw_ )
    {
        sfm::vec2i new_mouse_pos = sfm::Mouse::getPosition( renderWindow) - canvas_->getPos();
        if ( std::abs( new_mouse_pos.x - last_mouse_pos_.x) >= 10 || std::abs( new_mouse_pos.y - last_mouse_pos_.y) >= 10 )
        {
            last_mouse_pos_ = new_mouse_pos;
            drawLine( renderWindow, layer_, true);
        }
    }
    return true;
}


void Line::setAngleAndLength( sfm::vec2i begin_pos, sfm::vec2i end_pos)
{
    shape_.setPosition( begin_pos);
    shape_.setRotation( std::atan2( end_pos.y - begin_pos.y, end_pos.x - begin_pos.x) * 180 / M_PI);
    shape_.setSize( sfm::vec2u( len( begin_pos, end_pos), 5));
}


bool Line::isOnCanvas( sfm::vec2i mouse_pos)
{
    sfm::vec2i relative_pos = mouse_pos - canvas_->getPos();
    sfm::vec2u canvas_size = canvas_->getSize();

    return 0 <= relative_pos.x && relative_pos.x < static_cast<int>( canvas_size.x) &&
           0 <= relative_pos.y && relative_pos.y < static_cast<int>( canvas_size.y);
}


void Line::drawLine( const sfm::IRenderWindow *renderWindow, ILayer *layer, bool is_temp_layer)
{
    if ( !is_temp_layer )
    {
        canvas_->cleanTempLayer();
    }
    sfm::vec2i end_pos = sfm::Mouse::getPosition( renderWindow) - CANVAS_SECTOR_POS;

    setAngleAndLength( begin_pos_, end_pos);

    const sfm::IImage *image = shape_.getImage();
    sfm::vec2i offset = CANVAS_SECTOR_POS - canvas_->getPos();
    sfm::Color line_color = sfm::Color( 255, 0, 127);
    for ( size_t x = 0; x < CANVAS_SECTOR_SIZE.x; x++ )
    {
        for ( size_t y = 0; y < CANVAS_SECTOR_SIZE.y; y++ )
        {
            sfm::Color color = image->getPixel( x, y);
            if ( color.a != 0 )
            {
                layer->setPixel( sfm::vec2i( x + offset.x, y + offset.y), line_color);
            } else if ( is_temp_layer )
            {
                layer->setPixel( sfm::vec2i( x + offset.x, y + offset.y), sfm::Color( 0, 0, 0, 0));
            }
        }
    }
}
