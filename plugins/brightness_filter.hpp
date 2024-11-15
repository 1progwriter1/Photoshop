#ifndef PLUGIN_BRIGHTNESS_FILTER
#define PLUGIN_BRIGHTNESS_FILTER


#include "../headers/api_impl/bar.hpp"
#include "../api/api_canvas.hpp"


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


class BrightnessFilter : public ABarButton
{
    ICanvas *canvas_ = nullptr;
public:
    BrightnessFilter( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    bool update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event) override;
    void draw( sfm::IRenderWindow *renderWindow) override;
};


#endif // PLUGIN_BRIGHTNESS_FILTER
