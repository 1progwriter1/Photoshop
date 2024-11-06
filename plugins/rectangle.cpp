#include "rectangle.hpp"

#include <cassert>


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
    toolbar->addWindow( std::move( rectangle));

    return true;
}

void unloadPlugin()
{

}


Rectangle::Rectangle( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite) {}


void Rectangle::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
    parent_->finishButtonDraw( renderWindow, this);
}


bool Rectangle::update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event)
{
    return true;
}
