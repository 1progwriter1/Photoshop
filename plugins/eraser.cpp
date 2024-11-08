#include "eraser.hpp"
#include <cassert>


const wid_t kEraserButtonId = 2;
psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool loadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    psapi::IBar *toolbar = dynamic_cast<psapi::IBar *>( kRootWindowPtr->getWindowById( psapi::kToolBarWindowId));
    assert( toolbar && "Failed to cast to IBar" );

    std::unique_ptr<sfm::Texture> texture = std::make_unique<sfm::Texture>();
    std::unique_ptr<sfm::Sprite> sprite = std::make_unique<sfm::Sprite>();
    texture->loadFromFile("../images/eraser48_48.png");
    sprite->setTexture( texture.get());
    sprite->setPosition( 36, 132);

    std::unique_ptr<ABarButton> eraser = std::make_unique<Eraser>( kEraserButtonId, texture, sprite);
    eraser->setParent( toolbar);
    toolbar->addWindow( std::move( eraser));

    return true;
}

void unloadPlugin()
{

}


Eraser::Eraser( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite),
        canvas_( dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
}


bool Eraser::update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event)
{
    ABarButton::update( renderWindow, event);

    if ( getState() != psapi::IBarButton::State::Press )
        return true;

    sfm::vec2u size = canvas_->getSize();
    ILayer *cur_layer = canvas_->getLayer( canvas_->getActiveLayerIndex());
    assert( cur_layer );

    for ( unsigned int x = 0; x < size.x; x++ )
    {
        for ( unsigned int y = 0; y < size.y; y++ )
        {
            cur_layer->setPixel( vec2i( x, y), sfm::Color( 255, 255, 255, 255));
        }
    }

    setState( psapi::IBarButton::State::Normal);

    return true;
}


void Eraser::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
    parent_->finishButtonDraw( renderWindow, this);
}
