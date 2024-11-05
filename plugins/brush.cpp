#include "brush.hpp"
#include <cassert>
#include <iostream>


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

    std::unique_ptr<ABarButton> brush = std::make_unique<ABarButton>( texture, sprite);
    toolbar->addWindow( std::move( brush));

    return true;
}

void unloadPlugin()
{

}


Brush::Brush( std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   ABarButton( init_texture, init_sprite)
{
}


void Brush::draw( sfm::IRenderWindow *renderWindow)
{
    assert( 0 && "Not implemented" );
}


Brush::~Brush()
{
    std::cerr << "Brush destroyed!!!\n";
}
