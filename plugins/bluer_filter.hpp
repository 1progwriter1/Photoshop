#ifndef PLUGIN_BLUER_FILTER
#define PLUGIN_BLUER_FILTER


#include "../headers/api_impl/bar.hpp"
#include "../api/api_canvas.hpp"


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


struct Quat
{
    int r, g, b, a;

    Quat( int init_r, int init_g, int init_b, int init_a)
    :   r(init_r), g(init_g), b(init_b), a(init_a) {}

    Quat &operator+=(const Quat &y);
};


Quat operator+(const Quat &x, const Quat &y);


class BluerFilter : public ABarButton
{
    ICanvas *canvas_ = nullptr;
public:
    BluerFilter( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    bool update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event) override;
    void draw( sfm::IRenderWindow *renderWindow) override;
};



#endif // PLUGIN_BLUER_FILTER
