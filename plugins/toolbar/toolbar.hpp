#ifndef PLUGIN_TOOLBAR
#define PLUGIN_TOOLBAR


#include "api_impl/bar/bar.hpp"


extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}


class ToolBar : public ABar
{
public:
    ToolBar(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape,  std::unique_ptr<sfm::RectangleShape> &normal,
                                                            std::unique_ptr<sfm::RectangleShape> &onHover,
                                                            std::unique_ptr<sfm::RectangleShape> &pressed,
                                                            std::unique_ptr<sfm::RectangleShape> &released);

    sfm::vec2i calculateNextPos(sfm::vec2i init_pos) override;
};


#endif // PLUGIN_TOOLBAR
