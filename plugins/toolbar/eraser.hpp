#ifndef ERASER_PLUGIN
#define ERASER_PLUGIN


#include "api_impl/bar/button.hpp"
#include "api/api_canvas.hpp"


using namespace psapi;


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


class Eraser : public ABarButton
{
    ICanvas *canvas_;

    friend class EraserAction;
public:
    Eraser( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    void draw( sfm::IRenderWindow *renderWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;
private:
    void updateState(const IRenderWindow *render_window, const Event &event);
};


class EraserAction : public AUndoableAction
{
    Eraser *eraser_;
public:
    EraserAction(Eraser *eraser, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


#endif // ERASER_PLUGIN