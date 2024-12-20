#ifndef PLUGIN_TOOLBAR
#define PLUGIN_TOOLBAR


#include "api_impl/bar/bar.hpp"


extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}


class ToolBar : public ABarTextures
{
public:
    ToolBar(wid_t init_id, std::unique_ptr<sfm::IRectangleShape> main_shape,  BarTextures &textures);

    sfm::vec2i calculateNextPos(sfm::vec2i init_pos) override;
};


#endif // PLUGIN_TOOLBAR
