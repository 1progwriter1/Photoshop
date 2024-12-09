#ifndef PLUGINS_MENU
#define PLUGINS_MENU


#include "../headers/api_impl/bar.hpp"


extern "C"
{


bool loadPlugin();
void unloadPlugin();


class MenuBar : public ABar
{
    sfm::vec2i last_button_pos_ = {};
    sfm::vec2i offset_ = {};
public:
    MenuBar(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape,  std::unique_ptr<sfm::RectangleShape> &normal,
                                                            std::unique_ptr<sfm::RectangleShape> &onHover,
                                                            std::unique_ptr<sfm::RectangleShape> &pressed,
                                                            std::unique_ptr<sfm::RectangleShape> &released);

    sfm::vec2i calculateNextPos(sfm::vec2i init_pos) override;
    void addWindow(std::unique_ptr<IWindow> window) override;

};


}

#endif // PLUGINS_MENU
