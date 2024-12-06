#include "toolbar.hpp"
#include <iostream>
#include <memory>


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool loadPlugin()
{
    kRootWindowPtr = psapi::getRootWindow();

    std::unique_ptr<sfm::RectangleShape> main = std::make_unique<sfm::RectangleShape>();
    main->setPosition( psapi::getToolbarIntRect().pos + vec2i(5, 5));
    main->setSize( psapi::getToolbarIntRect().size - vec2u(10, 10));
    main->setFillColor( sfm::Color( 204, 255, 255));
    main->setOutlineColor( sfm::Color( 51, 153, 255));
    main->setOutlineThickness( 5);

    std::unique_ptr<sfm::RectangleShape> normal = std::make_unique<sfm::RectangleShape>();
    normal->setSize( vec2u( 48, 48));
    normal->setFillColor( sfm::Color());
    normal->setOutlineColor( sfm::Color());
    normal->setOutlineThickness( 5);

    std::unique_ptr<sfm::RectangleShape> onHover = std::make_unique<sfm::RectangleShape>();
    onHover->setSize( vec2u( 48, 48));
    onHover->setFillColor( sfm::Color());
    onHover->setOutlineColor( sfm::Color( 192, 192, 192));
    onHover->setOutlineThickness( 5);

    std::unique_ptr<sfm::RectangleShape> pressed = std::make_unique<sfm::RectangleShape>();
    pressed->setSize( vec2u( 48, 48));
    pressed->setFillColor( sfm::Color());
    pressed->setOutlineColor( sfm::Color( 51, 153, 255));
    pressed->setOutlineThickness( 5);

    std::unique_ptr<sfm::RectangleShape> released = std::make_unique<sfm::RectangleShape>();
    released->setSize( vec2u( 48, 48));
    released->setFillColor( sfm::Color());
    released->setOutlineColor( sfm::Color( 153, 204, 255));
    released->setOutlineThickness( 5);

    std::unique_ptr<psapi::IBar> bar = std::make_unique<ToolBar>( kToolBarWindowId, main,
                                                                normal,
                                                                onHover,
                                                                pressed,
                                                                released);

    kRootWindowPtr->addWindow( std::move( bar));

    return true;
}


void unloadPlugin()
{

}


ToolBar::ToolBar(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape,  std::unique_ptr<sfm::RectangleShape> &normal,
                                                            std::unique_ptr<sfm::RectangleShape> &onHover,
                                                            std::unique_ptr<sfm::RectangleShape> &pressed,
                                                            std::unique_ptr<sfm::RectangleShape> &released)
    :   Bar(init_id, main_shape, normal, onHover, pressed, released) {}


sfm::vec2i ToolBar::calculateNextPos(sfm::vec2i init_pos)
{
    size_t cnt_buttons = buttons_.size();
    int offset = (size_.x - buttons_size_.x) / 2 + pos_.x;

    sfm::vec2i pos = sfm::vec2i(offset, offset);
    pos.y += cnt_buttons * (buttons_size_.y + offset);

    return pos;
}
