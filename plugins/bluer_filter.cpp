#include "bluer_filter.hpp"
#include <cassert>


#include "../headers/windows_id.hpp"
#include "../headers/api_impl/canvas.hpp"


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool loadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    psapi::IBar *toolbar = dynamic_cast<psapi::IBar *>( kRootWindowPtr->getWindowById( psapi::kToolBarWindowId));
    assert( toolbar && "Failed to cast to IBar" );

    std::unique_ptr<sfm::Texture> texture = std::make_unique<sfm::Texture>();
    std::unique_ptr<sfm::Sprite> sprite = std::make_unique<sfm::Sprite>();
    texture->loadFromFile("../images/bluer_filter48_48.png");
    sprite->setTexture( texture.get());
    sprite->setPosition( 36, 804);

    std::unique_ptr<ABarButton> negative_filter = std::make_unique<BluerFilter>( kBluerFilterButtonId, texture, sprite);
    negative_filter->setParent( toolbar);
    toolbar->addWindow( std::move( negative_filter));

    return true;
}

void unloadPlugin()
{

}


BluerFilter::BluerFilter( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite), canvas_( dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
}


void BluerFilter::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
}


bool BluerFilter::update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event)
{
    ABarButton::update( renderWindow, event);

    if ( getState() != IBarButton::State::Press )
    {
        return true;
    }

    ILayer *layer = canvas_->getLayer( canvas_->getActiveLayerIndex());
    assert( layer );

    sfm::vec2u canvas_size = canvas_->getSize();

    for ( size_t x = 0; x < canvas_size.x - 2; x++ )
    {
        for ( size_t y = 0; y < canvas_size.y - 2; y++ )
        {
            int from_x = std::max( 0, static_cast<int>( x - 1));
            int to_x = std::min( static_cast<int>( canvas_size.x - 1), static_cast<int>( x + 1));

            int from_y = std::max( 0, static_cast<int>( y - 1));
            int to_y = std::min( static_cast<int>( canvas_size.y - 1), static_cast<int>( y + 1));

            int r = 0, g = 0, b = 0, a = 0;

            for ( int i = from_x; i <= to_x; i++ )
            {
                for ( int j = from_y; j <= to_y; j++ )
                {
                    sfm::Color color = layer->getPixel( sfm::vec2i( i, j));
                    r += color.r;
                    g += color.g;
                    b += color.b;
                    a += color.a;
                }
            }
            r /= 9;
            g /= 9;
            b /= 9;
            a /= 9;

            sfm::Color color( r, g, b, a);

            layer->setPixel( sfm::vec2i( x, y), color);
        }
    }

    setState( IBarButton::State::Normal);

    return true;
}


Quat operator+(const Quat &x, const Quat &y)
{
    return Quat(x.r + y.r, x.g + y.g, x.b + y.b, x.a + y.a);
}


Quat &Quat::operator+=(const Quat &y)
{
    this->r += y.r;
    this->g += y.g;
    this->b += y.b;
    this->a += y.a;
    return *this;
}
