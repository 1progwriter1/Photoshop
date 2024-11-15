#ifndef PLUGIN_BRUSH
#define PLUGIN_BRUSH


#include "../headers/api_impl/bar.hpp"
#include "../api/api_canvas.hpp"
#include <deque>


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


const wid_t kRedColorButtonId = 10;
const wid_t kBlueColorButtonId = 11;
const wid_t kGreenColorButtonId = 12;


class Brush : public ABarButton
{
    std::deque<sfm::vec2i> points_;
    sfm::vec2i last_pos_ = sfm::vec2i( 0, 0);

    ICanvas *canvas_;
    IBar *options_bar_;
    sfm::Color color_ = sfm::Color( 255, 0, 0);

    bool options_added_ = false;
    std::vector<std::unique_ptr<ABarButton>> options_;
public:
    Brush( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);
    ~Brush();

    bool update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event) override;
    void draw( sfm::IRenderWindow *renderWindow) override;
    void setColor( const sfm::Color &new_color);

private:
    void drawInterpolatedPoints( ILayer *layer, sfm::vec2i new_point);
    void drawPoint( ILayer *layer, sfm::vec2i pos);
    sfm::vec2i interpolate( sfm::vec2i p0, sfm::vec2i p1, sfm::vec2i p2, sfm::vec2i p3, float t);
    void addOptions();
    void removeOptions();
    void createOptions();
};


class ColorButton : public ABarButton
{
    Brush *brush_;
    sfm::Color color_ = sfm::Color( 255, 0, 0);
public:
    ColorButton( wid_t init_id, Brush *init_brush, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    void draw( sfm::IRenderWindow *renderWindow) override;
    bool update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event) override;
};


#endif // PLUGIN_BRUSH
