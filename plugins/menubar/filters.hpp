#ifndef FILTERS_FOR_MENU
#define FILTERS_FOR_MENU


#include "menu_items.hpp"
#include "api/api_canvas.hpp"
#include "api_impl/canvas.hpp"


class ControlPanel : public IWindow
{
    std::unique_ptr<TextButton> button_ok_;
    std::unique_ptr<TextButton> button_cancel_;

    std::unique_ptr<sfm::RectangleShape> shape_ = std::make_unique<sfm::RectangleShape>();
    std::unique_ptr<RectangleShape> normal_ = std::make_unique<sfm::RectangleShape>();
    std::unique_ptr<RectangleShape> onHover_ = std::make_unique<sfm::RectangleShape>();
    std::unique_ptr<RectangleShape> pressed_ = std::make_unique<sfm::RectangleShape>();
    std::unique_ptr<RectangleShape> released_ = std::make_unique<sfm::RectangleShape>();

    vec2i pos_ = vec2i(0, 0);
    vec2u size_ = vec2u(100, 100);
    vec2u buttons_rect_size_ = vec2u(0, 50);
    unsigned int min_buttons_width_ = 0;

    bool active_ = false;

    vec2u fitButtonsIn(vec2u size);

    friend class ControlPanelAction;
public:
    ControlPanel();
    virtual ~ControlPanel() = default;

    virtual void draw(IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    virtual wid_t getId() const override;

    virtual IWindow* getWindowById(wid_t id) override;
    virtual const IWindow* getWindowById(wid_t id) const override;

    virtual vec2i getPos() const override;
    virtual vec2u getSize() const override;

    virtual void setSize(const vec2u& size) override;
    virtual void setPos(const vec2i& pos) override;
    virtual void setParent(const IWindow* parent) override;

    virtual void forceActivate() override;
    virtual void forceDeactivate() override;

    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

    int getButtonsRectHeight() const;
    void finishButtonDraw(IRenderWindow* renderWindow, const TextButton* button);

    ABarButton::State getOkState() const;
    ABarButton::State getCancelState() const;
    void unsetAll();
};


class ControlPanelAction : public AAction
{
    ControlPanel *button_;
public:
    ControlPanelAction(ControlPanel *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


class BareliefFilter : public TextButton
{
    friend class BareliefFilterAction;
public:
    BareliefFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class BareliefFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    BareliefFilter *button_;
public:
    BareliefFilterAction(BareliefFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};

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


sfm::Color createContrastColor( const sfm::Color &normal, const sfm::Color &blurred, int cf);


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


class BlurFilter : public TextButton
{
    friend class BlurFilterAction;
public:
    BlurFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class BlurFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    BlurFilter *button_;
public:
    BlurFilterAction(BlurFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


class GaussBlurFilter : public TextButton
{
    friend class GaussBlurFilterAction;
public:
    GaussBlurFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class GaussBlurFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    GaussBlurFilter *button_;
public:
    GaussBlurFilterAction(GaussBlurFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


class NegativeFilter : public TextButton
{
    friend class NegativeFilterAction;
public:
    NegativeFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class NegativeFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    NegativeFilter *button_;
public:
    NegativeFilterAction(NegativeFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


class BrightnessFilter : public TextButton
{
    friend class BrightnessFilterAction;
public:
    BrightnessFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class BrightnessFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    BrightnessFilter *button_;
public:
    BrightnessFilterAction(BrightnessFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


#endif // FILTERS_FOR_MENU
