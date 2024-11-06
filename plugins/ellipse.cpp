#include "ellipse.hpp"

#include <cassert>


psapi::IWindowContainer *kRootWindowPtr = nullptr;
const wid_t kEllipseButtonId = 4;


bool loadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    psapi::IBar *toolbar = dynamic_cast<psapi::IBar *>( kRootWindowPtr->getWindowById( psapi::kToolBarWindowId));
    assert( toolbar && "Failed to cast to IBar" );

    std::unique_ptr<sfm::Texture> texture = std::make_unique<sfm::Texture>();
    std::unique_ptr<sfm::Sprite> sprite = std::make_unique<sfm::Sprite>();
    texture->loadFromFile("../images/ellipse48_48.png");
    sprite->setTexture( texture.get());
    sprite->setPosition( 36, 324);

    std::unique_ptr<ABarButton> ellipse = std::make_unique<Ellipse>( kEllipseButtonId, texture, sprite);
    toolbar->addWindow( std::move( ellipse));

    return true;
}

void unloadPlugin()
{

}


Ellipse::Ellipse( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite)
{}



