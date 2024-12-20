#include "toolbar.hpp"
#include <iostream>
#include <memory>


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool onLoadPlugin()
{
    kRootWindowPtr = psapi::getRootWindow();

    std::unique_ptr<sfm::IRectangleShape> main = sfm::IRectangleShape::create(0, 0);
    main->setPosition( psapi::getToolbarIntRect().pos + vec2i(1, 1));
    main->setSize( psapi::getToolbarIntRect().size - vec2u(2, 2));
    main->setFillColor( sfm::Color( 128, 128, 128));
    main->setOutlineColor( sfm::Color( 64, 64, 64));
    main->setOutlineThickness( 1);

    BarTextures textures;
    textures.texture_normal = sfm::ITexture::create();
    textures.texture_on_hover = sfm::ITexture::create();
    textures.texture_pressed = sfm::ITexture::create();
    textures.texture_released = sfm::ITexture::create();

    textures.texture_normal->loadFromFile("../assets/images/normal.png");
    textures.texture_on_hover->loadFromFile("../assets/images/on_hover.png");
    textures.texture_pressed->loadFromFile("../assets/images/pressed.png");
    textures.texture_released->loadFromFile("../assets/images/normal.png");

    textures.sprite_normal = sfm::ISprite::create();
    textures.sprite_on_hover = sfm::ISprite::create();
    textures.sprite_pressed = sfm::ISprite::create();
    textures.sprite_released = sfm::ISprite::create();

    textures.sprite_normal->setTexture(textures.texture_normal.get());
    textures.sprite_on_hover->setTexture(textures.texture_on_hover.get());
    textures.sprite_pressed->setTexture(textures.texture_pressed.get());
    textures.sprite_released->setTexture(textures.texture_released.get());

    std::unique_ptr<psapi::IBar> bar = std::make_unique<ToolBar>( kToolBarWindowId, std::move(main), textures);

    kRootWindowPtr->addWindow( std::move( bar));

    return true;
}


void onUnloadPlugin()
{

}


ToolBar::ToolBar(wid_t init_id, std::unique_ptr<sfm::IRectangleShape> main_shape, BarTextures &textures)
    :   ABarTextures(init_id, std::move(main_shape), textures)
{
    buttons_size_ = sfm::vec2u(32, 32);
}


sfm::vec2i ToolBar::calculateNextPos(sfm::vec2i init_pos)
{
    size_t cnt_buttons = buttons_.size();
    int offset = (getSize().x - buttons_size_.x) / 2 + getPos().x;

    sfm::vec2i pos = sfm::vec2i(offset, offset);
    pos.y += cnt_buttons * (buttons_size_.y + offset);

    return pos;
}
