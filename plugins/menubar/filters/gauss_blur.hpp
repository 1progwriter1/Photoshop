#ifndef GAUSS_BLUR_FILTER_PLUGIN
#define GAUSS_BLUR_FILTER_PLUGIN


#include "../menu_items.hpp"
#include "api_impl/canvas.hpp"
#include "../control_panel.hpp"


extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}


class GaussBlurControlPanel;


class GaussBlurFilter : public TextButton
{
    std::unique_ptr<GaussBlurControlPanel> control_panel_;

    bool is_applied_ = false;
    bool is_ok_ = false;
    bool is_cancel_ = false;

    std::unique_ptr<IMementable<ILayerSnapshot>> memento_;
    std::unique_ptr<ILayerSnapshot> snapshot_;

    friend class GaussBlurFilterAction;
public:
    GaussBlurFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void draw(IRenderWindow* renderWindow) override;

    void updateState(const IRenderWindow *renderWindow, const Event &event);
    void updatePanel(const IRenderWindow *renderWindow, const Event &event);

    void setOk(bool value);
    void setCancel(bool value);
    void unsetAll();
};


class GaussBlurFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    GaussBlurFilter *button_;

    ILayer *main_layer_ = nullptr;
    ILayer *tmp_layer_ = nullptr;

    void applyToTmpLayer();
    void applyToMainLayer();
    void removeTmpLayer();
public:
    GaussBlurFilterAction(GaussBlurFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


class GaussBlurControlPanel : public ControlPanel
{
    friend class UnsharpMaskControlPanelAction;

    GaussBlurFilter *parent_;

    friend class GaussBlurControlPanelAction;
public:
    GaussBlurControlPanel();

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void draw(IRenderWindow* renderWindow) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);

    void setFilter(GaussBlurFilter *parent);
};


class GaussBlurControlPanelAction : public AAction
{
    GaussBlurControlPanel *button_;
public:
    GaussBlurControlPanelAction(GaussBlurControlPanel *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


#endif // GAUSS_BLUR_FILTER_PLUGIN
