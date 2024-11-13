#include "line.hpp"
#include <cassert>


psapi::IWindowContainer *kRootWindowPtr = nullptr;
const wid_t kLineButtonId = 5;


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
    :   ABarButton( init_id, init_texture, init_sprite)
{}


void Line::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
    parent_->finishButtonDraw( renderWindow, this);
}


bool Line::update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event)
{
    ABarButton::update( renderWindow, event);
    return true;
}
