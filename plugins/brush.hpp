#ifndef PLUGIN_BRUSH
#define PLUGIN_BRUSH


#include "../headers/api_impl/bar.hpp"


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


class Brush : public ABarButton
{
public:
    Brush( std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);
    ~Brush();

    void draw( sfm::IRenderWindow *renderWindow) override;
};


#endif // PLUGIN_BRUSH
