#include "eraser.hpp"

#include <cassert>


psapi::IWindowContainer *kRootWindowPtr = nullptr;


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

    std::unique_ptr<ABarButton> rectangle = std::make_unique<ABarButton>( texture, sprite);
    toolbar->addWindow( std::move( rectangle));

    return true;
}

void unloadPlugin()
{

}
