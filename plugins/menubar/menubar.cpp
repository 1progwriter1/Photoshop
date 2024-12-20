#include "menubar.hpp"
#include <memory>


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool onLoadPlugin()
{
    kRootWindowPtr = psapi::getRootWindow();

    sfm::IntRect rect = psapi::getMenuBarIntRect();

    std::unique_ptr<sfm::IRectangleShape> main = sfm::IRectangleShape::create(0, 0);
    main->setPosition( sfm::vec2i( rect.pos + sfm::vec2i(1, 1)));
    main->setSize( sfm::vec2u( rect.size - sfm::vec2u( 2, 2)));
    main->setFillColor( sfm::Color( 128, 128, 128));
    main->setOutlineColor( sfm::Color( 64, 64, 64));
    main->setOutlineThickness( 1);

    std::unique_ptr<sfm::IRectangleShape> normal = sfm::IRectangleShape::create(0, 0);
    normal->setFillColor( sfm::Color());

    std::unique_ptr<sfm::IRectangleShape> onHover = sfm::IRectangleShape::create(0, 0);
    onHover->setFillColor( sfm::Color());

    std::unique_ptr<sfm::IRectangleShape> pressed = sfm::IRectangleShape::create(0, 0);
    pressed->setFillColor( sfm::Color());

    std::unique_ptr<sfm::IRectangleShape> released = sfm::IRectangleShape::create(0, 0);
    released->setFillColor( sfm::Color());

    BarRectangleShapes shapes;
    shapes.normal = std::move( normal);
    shapes.on_hover = std::move( onHover);
    shapes.pressed = std::move( pressed);
    shapes.released = std::move( released);

    std::unique_ptr<psapi::IBar> bar = std::make_unique<MenuBar>( kMenuBarWindowId, std::move(main), shapes);

    bar->setParent( kRootWindowPtr);

    kRootWindowPtr->addWindow( std::move( bar));

    return true;
}


void onUnloadPlugin()
{

}


MenuBar::MenuBar(wid_t init_id, std::unique_ptr<sfm::IRectangleShape> main_shape,  BarRectangleShapes &shapes)
    :   ABarShapes(init_id, std::move(main_shape), shapes)
{
    offset_ = ABar::getPos();
    buttons_size_ = sfm::vec2u(0, 32);
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
    window->setSize(sfm::vec2u(0, 32));
    window->setPos(calculateNextPos(window->getPos()));
    buttons_.push_back( std::unique_ptr<IBarButton>( static_cast<IBarButton *>( window.release())));
}
