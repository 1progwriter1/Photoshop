#ifndef PLUGIN_CONTRAST_FILTER
#define PLUGIN_CONTRAST_FILTER


#include "../menu_items.hpp"
#include "api_impl/canvas.hpp"
#include "../control_panel.hpp"


class ContrastControlPanel;


class ContrastFilter : public TextButton
{
    std::unique_ptr<ContrastControlPanel> control_panel_;

    bool is_applied_ = false;
    bool is_ok_ = false;
    bool is_cancel_ = false;

    friend class ContrastFilterAction;
public:
    ContrastFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void draw(IRenderWindow* renderWindow) override;

    void updateState(const IRenderWindow *renderWindow, const Event &event);
    void updatePanel(const IRenderWindow *renderWindow, const Event &event);

    void setOk(bool value);
    void setCancel(bool value);
    void unsetAll();
};



class ContrastFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    ContrastFilter *button_;

    Layer *main_layer_ = nullptr;
    Layer *tmp_layer_ = nullptr;

    void applyToTmpLayer();
    void applyToMainLayer();
    void removeTmpLayer();
public:
    ContrastFilterAction(ContrastFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


class ContrastControlPanel : public ControlPanel
{
    friend class ContrastControlPanelAction;

    std::unique_ptr<sfm::RectangleShape> main_shape_ = std::make_unique<sfm::RectangleShape>();
    std::unique_ptr<sfm::RectangleShape> line_shape_ = std::make_unique<sfm::RectangleShape>();
    std::unique_ptr<sfm::RectangleShape> slider_shape_ = std::make_unique<sfm::RectangleShape>();

    ContrastFilter *parent_;

    friend class ControlPanelAction;
public:
    ContrastControlPanel();

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void draw(IRenderWindow* renderWindow) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);

    void setSize(const vec2u& size) override;
    void setPos(const vec2i& pos) override;
    void setFilter(ContrastFilter *parent);

    ABarButton::State getOkState() const;
    ABarButton::State getCancelState() const;
};


class ContrastControlPanelAction : public AAction
{
    ContrastControlPanel *button_;
public:
    ContrastControlPanelAction(ContrastControlPanel *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


sfm::Color createContrastColor( const sfm::Color &normal, const sfm::Color &blurred, int cf);


#endif // PLUGIN_CONTRAST_FILTER
