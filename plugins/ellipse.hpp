#ifndef ELLIPSE_PLUGIN
#define ELLIPSE_PLUGIN


#include "../headers/api_impl/bar.hpp"


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


class Ellipse : public ABarButton
{
public:
    Ellipse( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);
};


#endif // ELLIPSE_PLUGIN
