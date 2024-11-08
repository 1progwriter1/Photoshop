#ifndef RECTANGLE_PLUGIN
#define RECTANGLE_PLUGIN


#include "../headers/api_impl/bar.hpp"
#include "../api/api_canvas.hpp"


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


class Rectangle : public ABarButton
{
    sfm::vec2i left_upper_edge_ = sfm::vec2i();
    sfm::vec2u rect_size_ = sfm::vec2u();

    bool draw_ = false;

    ICanvas *canvas_;
    ILayer *layer_ = nullptr;
public:
    Rectangle( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    void draw( sfm::IRenderWindow *renderWindow) override;
    bool update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event) override;

    void drawFinalVersion( const sfm::IRenderWindow *renderWindow);
    void drawTempVersion( const sfm::IRenderWindow *renderWindow);
    bool isOnCanvas( sfm::vec2i mouse_pos);
};



#endif // RECTANGLE_PLUGIN
