#ifndef PLUGIN_FILTER
#define PLUGIN_FILTER


#include "../headers/api_impl/bar.hpp"
#include "../api/api_canvas.hpp"


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


class FilterButton : public ABarButton
{
    ICanvas *canvas_ = nullptr;
    IBar *options_bar_;

    bool options_added_ = false;
    std::vector<std::unique_ptr<ABarButton>> options_;
    std::vector<wid_t> id_;
public:
    FilterButton( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    bool update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event) override;
    void draw( sfm::IRenderWindow *renderWindow) override;

    void addOptions();
    void removeOptions();
    void createOptions();
};


class AFilter : public ABarButton
{
public:
    AFilter( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);
    virtual ~AFilter() = default;
};


class ContrastFilter : public AFilter
{
    ICanvas *canvas_ = nullptr;
public:
    ContrastFilter( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    bool update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event) override;
    void draw( sfm::IRenderWindow *renderWindow) override;
};

sfm::Color createContrastColor( const sfm::Color &normal, const sfm::Color &blurred);

class GaussBluerFilter : public AFilter
{
    ICanvas *canvas_ = nullptr;
public:
    GaussBluerFilter( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    bool update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event) override;
    void draw( sfm::IRenderWindow *renderWindow) override;
};


class BareliefFilter : public AFilter
{
    ICanvas *canvas_ = nullptr;
public:
    BareliefFilter( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    bool update( const sfm::IRenderWindow *renderWindow, const sfm::Event &event) override;
    void draw( sfm::IRenderWindow *renderWindow) override;
};


#endif // PLUGIN_FILTER
