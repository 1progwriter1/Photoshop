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
    sfm::vec2i last_mouse_pos_ = sfm::vec2i();
    sfm::Color color_ = sfm::Color( 255, 0, 0);

    bool draw_ = false;

    ICanvas *canvas_;
    ILayer *layer_ = nullptr;
    IBar *options_bar_;

    bool options_added_ = false;
    std::vector<std::unique_ptr<ABarButton>> options_;
    std::vector<wid_t> id_;
public:
    Rectangle( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    void draw( sfm::IRenderWindow *renderWindow) override;
    bool update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event) override;

    void drawRectangle( const sfm::IRenderWindow *renderWindow, ILayer *layer, bool is_temp_layer);
    bool isOnCanvas( sfm::vec2i mouse_pos);

    void setColor( const sfm::Color &new_color);
    const sfm::Color &getColor() const;

    void addOptions();
    void removeOptions();
    void createOptions();
};


class ColorButton : public ABarButton
{
    Rectangle *rect_;
    sfm::Color color_ = sfm::Color( 255, 0, 0);
    bool is_set_ = false;
public:
    ColorButton( wid_t init_id, Rectangle *init_rect, const sfm::Color &init_color,
                std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    void draw( sfm::IRenderWindow *renderWindow) override;
    bool update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event) override;
};


#endif // RECTANGLE_PLUGIN
