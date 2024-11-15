#include "negative_filter.hpp"
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
    texture->loadFromFile("../images/negative_filter48_48.png");
    sprite->setTexture( texture.get());
    sprite->setPosition( 36, 612);

    std::unique_ptr<ABarButton> negative_filter = std::make_unique<NegativeFilter>( kNegativeFilterButtonId, texture, sprite);
    negative_filter->setParent( toolbar);
    toolbar->addWindow( std::move( negative_filter));

    return true;
}

void unloadPlugin()
{

}


NegativeFilter::NegativeFilter( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite), canvas_( dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
}


void NegativeFilter::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
}


bool NegativeFilter::update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event)
{
    ABarButton::update( renderWindow, event);

    if ( getState() != IBarButton::State::Press )
    {
        return true;
    }

    ILayer *layer = canvas_->getLayer( canvas_->getActiveLayerIndex());
    assert( layer );

    sfm::vec2u canvas_size = canvas_->getSize();

    for ( size_t x = 0; x < canvas_size.x; x++ )
    {
        for ( size_t y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color color = layer->getPixel( sfm::vec2i( x, y));
            color.r = 255 - color.r;
            color.g = 255 - color.g;
            color.b = 255 - color.b;
            layer->setPixel( sfm::vec2i( x, y), color);
        }
    }

    setState( IBarButton::State::Normal);

    return true;
}
