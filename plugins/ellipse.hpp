#ifndef ELLIPSE_PLUGIN
#define ELLIPSE_PLUGIN


#include "../headers/api_impl/bar.hpp"
#include "../api/api_canvas.hpp"


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


class Ellipse : public ABarButton
{
    bool draw_ = false;
    sfm::vec2i left_upper_edge_ = sfm::vec2i();
    sfm::vec2i last_mouse_pos_ = sfm::vec2i();
    ILayer *layer_ = nullptr;
    ICanvas *canvas_ = nullptr;

    friend class EllipseAction;
public:
    Ellipse( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;
    bool isOnCanvas( sfm::vec2i mouse_pos);
    void drawEllipse( const sfm::IRenderWindow *renderWindow, ILayer *layer, bool is_temp_layer);
    bool isOnEllipse( sfm::vec2i pos, sfm::vec2u size2, sfm::vec2i center);

    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class EllipseAction : public AUndoableAction
{
    Ellipse *ellipse_;
public:
    EllipseAction(Ellipse *ellipse, const IRenderWindow *renderWindow, const Event &event);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};

#endif // ELLIPSE_PLUGIN
