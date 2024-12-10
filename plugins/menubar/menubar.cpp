#include "menubar.hpp"
#include <memory>


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool loadPlugin()
{
    kRootWindowPtr = psapi::getRootWindow();

    sfm::IntRect rect = psapi::getMenuBarIntRect();

    std::unique_ptr<sfm::RectangleShape> main = std::make_unique<sfm::RectangleShape>();
    main->setPosition( sfm::vec2i( rect.pos + sfm::vec2i( 5, 5)));
    main->setSize( sfm::vec2u( rect.size - sfm::vec2u( 10, 10)));
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

    std::unique_ptr<psapi::IBar> bar = std::make_unique<MenuBar>( kMenuBarWindowId, main,
                                                                normal,
                                                                onHover,
                                                                pressed,
                                                                released);

    bar->setParent( kRootWindowPtr);

    kRootWindowPtr->addWindow( std::move( bar));

    return true;
}


void unloadPlugin()
{

}


MenuBar::MenuBar(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape,  std::unique_ptr<sfm::RectangleShape> &normal,
                                                            std::unique_ptr<sfm::RectangleShape> &onHover,
                                                            std::unique_ptr<sfm::RectangleShape> &pressed,
                                                            std::unique_ptr<sfm::RectangleShape> &released)
    :   ABar(init_id, main_shape, normal, onHover, pressed, released)
{
    offset_ = ABar::getPos();
    int offset = (ABar::getSize().y - buttons_size_.y) / 2;
    offset_ += sfm::vec2i(offset, offset);
}


sfm::vec2i MenuBar::calculateNextPos(sfm::vec2i init_pos)
{
    sfm::vec2i pos = offset_;
    for ( auto &item : buttons_ )
    {
        sfm::vec2i end_pos = item->getPos() + sfm::vec2i(item->getSize());
        pos.x = std::max(pos.x, end_pos.x);
    }
    pos.y = offset_.y;
    pos.x += offset_.x - ABar::getPos().x;
    return pos;
}


void MenuBar::addWindow(std::unique_ptr<IWindow> window)
{
    sfm::vec2u size = window->getSize();
    window->setSize(sfm::vec2u(0, getSize().y * 0.65));
    window->setPos(calculateNextPos(window->getPos()));
    buttons_.push_back( std::unique_ptr<IBarButton>( static_cast<IBarButton *>( window.release())));
}
