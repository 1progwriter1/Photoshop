#include "brush.hpp"
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
    texture->loadFromFile("../images/brush48_48.png");
    sprite->setTexture( texture.get());
    sprite->setPosition( 36, 36);

    std::unique_ptr<ABarButton> brush = std::make_unique<Brush>( kBrushButtonId, texture, sprite);
    brush->setParent( toolbar);
    toolbar->addWindow( std::move( brush));

    return true;
}

void unloadPlugin()
{

}


Brush::Brush( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite),
        canvas_( dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId))),
        options_bar_( dynamic_cast<IBar *>( getRootWindow()->getWindowById( kOptionsBarWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
    assert( options_bar_ && "Failed to cast to options bar" );
}


Brush::~Brush()
{

}


bool Brush::update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event)
{
    ABarButton::update( renderWindow, event);

    if ( getState() != IBarButton::State::Press )
        return true;

    if ( !( sfm::Mouse::isButtonPressed( sfm::Mouse::Button::Left) || sfm::Mouse::isButtonPressed( sfm::Mouse::Button::Right) ) )
    {
        points_.clear();
        return true;
    }

    sfm::vec2i mouse_pos = sfm::Mouse::getPosition( renderWindow);
    sfm::vec2i relative_pos = mouse_pos - canvas_->getPos();

    drawInterpolatedPoints( canvas_->getLayer( canvas_->getActiveLayerIndex()), relative_pos);

    return true;
}


void Brush::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
    parent_->finishButtonDraw( renderWindow, this);
}


void Brush::drawPoint( ILayer *layer, sfm::vec2i pos)
{
    int radius = 2;
    int radius2 = 4;

    for ( int x = -radius; x < radius; x++ )
    {
        for ( int y = -radius; y < radius; y++ )
        {
            if ( x * x + y * y <= radius2 )
            {
                layer->setPixel( pos, sfm::Color( 255, 0, 0));
            }
            pos.y++;
        }
        pos.y -= 2 * radius;
        pos.x++;
    }
}


sfm::vec2i Brush::interpolate( sfm::vec2i p0, sfm::vec2i p1, sfm::vec2i p2, sfm::vec2i p3, float t)
{
    double t2   = t  * t,
           t3   = t2 * t,
           t3_2 = t3 * 2,
           t2_3 = t2 * 3;

    double h00 =  t3_2 - t2_3 + 1;
    double h10 =  t3   - t2 * 2 + t;
    double h01 = -t3_2 + t2_3;
    double h11 =  t3   - t2;

    sfm::vec2f p0f = sfm::vec2f( p0.x, p0.y);
    sfm::vec2f p1f = sfm::vec2f( p1.x, p1.y);
    sfm::vec2f p2f = sfm::vec2f( p2.x, p2.y);
    sfm::vec2f p3f = sfm::vec2f( p3.x, p3.y);

    sfm::vec2f m0 = (p2f - p0f) * 0.5;
    sfm::vec2f m1 = (p3f - p1f) * 0.5;

    sfm::vec2f res = h00 * p1f + h10 * m0 + h01 * p2f + h11 * m1;
    return sfm::vec2i( std::round( res.x), std::round( res.y));
}


void Brush::drawInterpolatedPoints( ILayer *layer, sfm::vec2i new_point)
{
    points_.push_back( new_point);

    if ( points_.size() < 4 )
    {
        drawPoint( layer, new_point);
        return;
    }

    sfm::vec2i &p0 = points_[0];
    sfm::vec2i &p1 = points_[1];
    sfm::vec2i &p2 = points_[2];
    sfm::vec2i &p3 = points_[3];

    float steps = static_cast<float>( std::min<double>( sfm::len( p0, p1) + sfm::len( p1, p2) + sfm::len( p2, p3), 100.f));

    for ( float t = 0; t < steps; t += 0.01 )
    {
        drawPoint( layer, interpolate( p0, p1, p2, p3, t - size_t( t)));
    }

    points_.pop_front();
}


void Brush::addOptions()
{

}


void Brush::removeOptions()
{

}
