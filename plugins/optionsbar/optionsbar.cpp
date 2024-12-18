#include "optionsbar.hpp"
#include <memory>


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool onLoadPlugin()
{
    kRootWindowPtr = psapi::getRootWindow();

    sfm::IntRect rect = psapi::getOptionsBarIntRect();

    std::unique_ptr<sfm::RectangleShape> main = std::make_unique<sfm::RectangleShape>();
    main->setPosition( sfm::vec2i( rect.pos + sfm::vec2i( 5, 5)));
    main->setSize( sfm::vec2u( rect.size - sfm::vec2u( 10, 10)));
    main->setFillColor( sfm::Color( 204, 255, 255));
    main->setOutlineColor( sfm::Color( 51, 153, 255));
    main->setOutlineThickness( 5);

    std::unique_ptr<psapi::IOptionsBar> bar = std::make_unique<OptionsBar>( kOptionsBarWindowId, main);
    bar->setParent( kRootWindowPtr);
    kRootWindowPtr->addWindow( std::move( bar));

    return true;
}


void onUnloadPlugin()
{

}


OptionsBar::OptionsBar(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape)
    :   AOptionsBar(init_id, main_shape) {}


sfm::vec2i OptionsBar::calculateNextPos(sfm::vec2i init_pos)
{
    size_t cnt_options = options_.size();
    int offset = (size_.x - option_size_.x) / 2 + pos_.x;

    sfm::vec2i pos = sfm::vec2i(offset, offset);
    pos.y += cnt_options * (option_size_.y + offset);

    return pos;
}
