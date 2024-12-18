#ifndef PLUGIN_CONTRAST_FILTER
#define PLUGIN_CONTRAST_FILTER


#include "../menu_items.hpp"
#include "api_impl/canvas.hpp"
#include "../control_panel.hpp"


class UnsharpMaskControlPanel;


class UnsharpMaskFilter : public TextButton
{
    std::unique_ptr<UnsharpMaskControlPanel> control_panel_;

    bool is_applied_ = false;
    bool is_ok_ = false;
    bool is_cancel_ = false;

    std::unique_ptr<IMementable<ILayerSnapshot>> memento_;
    std::unique_ptr<ILayerSnapshot> snapshot_;

    friend class UnsharpMaskFilterAction;
public:
    UnsharpMaskFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void draw(IRenderWindow* renderWindow) override;

    void updateState(const IRenderWindow *renderWindow, const Event &event);
    void updatePanel(const IRenderWindow *renderWindow, const Event &event);

    void setOk(bool value);
    void setCancel(bool value);
    void unsetAll();
};



class UnsharpMaskFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    UnsharpMaskFilter *button_;

    ILayer *main_layer_ = nullptr;
    ILayer *tmp_layer_ = nullptr;

    void applyToTmpLayer();
    void applyToMainLayer();
    void removeTmpLayer();
public:
    UnsharpMaskFilterAction(UnsharpMaskFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


class UnsharpMaskControlPanel : public ControlPanel
{
    friend class UnsharpMaskControlPanelAction;

    std::unique_ptr<sfm::RectangleShape> main_shape_ = std::make_unique<sfm::RectangleShape>();
    std::unique_ptr<sfm::RectangleShape> line_shape_ = std::make_unique<sfm::RectangleShape>();
    std::unique_ptr<sfm::RectangleShape> slider_shape_ = std::make_unique<sfm::RectangleShape>();

    UnsharpMaskFilter *parent_;

    friend class ControlPanelAction;
public:
    UnsharpMaskControlPanel
();

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void draw(IRenderWindow* renderWindow) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);

    void setSize(const vec2u& size) override;
    void setPos(const vec2i& pos) override;
    void setFilter(UnsharpMaskFilter *parent);
};


class UnsharpMaskControlPanelAction : public AAction
{
    UnsharpMaskControlPanel *button_;
public:
    UnsharpMaskControlPanelAction(UnsharpMaskControlPanel *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


sfm::Color createContrastColor( const sfm::Color &normal, const sfm::Color &blurred, int cf);


#endif // PLUGIN_CONTRAST_FILTER
