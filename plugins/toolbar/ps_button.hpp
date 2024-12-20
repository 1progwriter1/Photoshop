#ifndef PS_BUTTON_BRUSH
#define PS_BUTTON_BRUSH


#include "headers/api_impl/bar/button.hpp"
#include "api/api_canvas.hpp"
#include <deque>


using namespace psapi;


extern "C"
{
    bool onLoadPlugin();
    void onUnloadPlugin();
}


class PsButton : public ABarButton
{
    friend class PsButtonAction;
public:
    PsButton(wid_t init_id, std::unique_ptr<sfm::ITexture> init_texture, std::unique_ptr<sfm::ISprite> init_sprite);

    void updateState(const IRenderWindow *render_window, const Event &event);
    std::unique_ptr<IAction> createAction(const IRenderWindow *render_window, const Event &event) override;
};


class PsButtonAction : public AAction
{
    PsButton *button_;
public:
    PsButtonAction(PsButton *button, const IRenderWindow *render_window, const Event &event);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;
};


#endif // PS_BUTTON_BRUSH
