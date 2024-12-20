#ifndef PLUGINS_MENU
#define PLUGINS_MENU


#include "api_impl/bar/bar.hpp"


extern "C"
{


bool onLoadPlugin();
void onUnloadPlugin();


}


class MenuBar : public ABarShapes
{
    sfm::vec2i last_button_pos_ = {};
    sfm::vec2i offset_ = {};
public:
    MenuBar(wid_t init_id, std::unique_ptr<sfm::IRectangleShape> main_shape, BarRectangleShapes &shapes);

    sfm::vec2i calculateNextPos(sfm::vec2i init_pos) override;
    void addWindow(std::unique_ptr<IWindow> window) override;
};


#endif // PLUGINS_MENU
