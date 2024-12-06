#ifndef PLUGIN_TOOLBAR
#define PLUGIN_TOOLBAR


#include "../headers/api_impl/bar.hpp"


extern "C"
{

bool loadPlugin();
void unloadPlugin();

}


class ToolBar : public Bar
{
public:
    ToolBar(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape,  std::unique_ptr<sfm::RectangleShape> &normal,
                                                            std::unique_ptr<sfm::RectangleShape> &onHover,
                                                            std::unique_ptr<sfm::RectangleShape> &pressed,
                                                            std::unique_ptr<sfm::RectangleShape> &released);

    sfm::vec2i calculateNextPos(sfm::vec2i init_pos) override;
};


#endif // PLUGIN_TOOLBAR
