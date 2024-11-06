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


class Brush : public ABarButton
{
    std::deque<sfm::vec2i> points_;
public:
    Brush( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);
    ~Brush();

    bool update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event) override;
    void draw( sfm::IRenderWindow *renderWindow) override;

    void drawInterpolatedPoints( ILayer *layer, sfm::vec2i new_point);
    void drawPoint( ILayer *layer, sfm::vec2i pos);
    sfm::vec2i interpolate( sfm::vec2i p0, sfm::vec2i p1, sfm::vec2i p2, sfm::vec2i p3, float t);
};


#endif // PLUGIN_BRUSH
