#include "barelief_filter.hpp"
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
    texture->loadFromFile("../images/barelief_filter48_48.png");
    sprite->setTexture( texture.get());
    sprite->setPosition( 36, 708);

    std::unique_ptr<ABarButton> barelief_filter = std::make_unique<BareliefFilter>( kBareliefFilterButtonId, texture, sprite);
    barelief_filter->setParent( toolbar);
    toolbar->addWindow( std::move( barelief_filter));

    return true;
}

void unloadPlugin()
{

}


BareliefFilter::BareliefFilter( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite), canvas_( dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
}


void BareliefFilter::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
}


bool BareliefFilter::update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event)
{
    ABarButton::update( renderWindow, event);

    if ( getState() != IBarButton::State::Press )
    {
        return true;
    }

    ILayer *layer = canvas_->getLayer( canvas_->getActiveLayerIndex());
    assert( layer );

    sfm::vec2u canvas_size = canvas_->getSize();

    for ( unsigned x = 0; x < canvas_size.x; x++ )
    {
        for ( unsigned y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color color_1 = layer->getPixel( sfm::vec2i( std::min( x + 4, canvas_size.x - 1), y));
            sfm::Color color_2 = layer->getPixel( sfm::vec2i( x, std::min( y + 4, canvas_size.y - 1)));

            sfm::Color color;
            color.r = (255 - color_1.r + color_2.r) / 2;
            color.g = (255 - color_1.g + color_2.g) / 2;
            color.b = (255 - color_1.b + color_2.b) / 2;
            color.a = (255 - color_1.a + color_2.a) / 2;
            layer->setPixel( sfm::vec2i( x, y), color);
        }
    }

    setState( IBarButton::State::Normal);

    return true;
}
