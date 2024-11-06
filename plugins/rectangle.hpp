#ifndef RECTANGLE_PLUGIN
#define RECTANGLE_PLUGIN


#include "../headers/api_impl/bar.hpp"


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


class Rectangle : public ABarButton
{
public:
    Rectangle( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    void draw( sfm::IRenderWindow *renderWindow) override;
    bool update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event) override;
};



#endif // RECTANGLE_PLUGIN
