#ifndef PLUGINS_CONTROL_PANEL
#define PLUGINS_CONTROL_PANEL


#include "api/api_photoshop.hpp"
#include "api_impl/sfm.hpp"
#include "menu_items.hpp"


class ControlPanel : public IWindow
{
    std::unique_ptr<TextButton> button_ok_;
    std::unique_ptr<TextButton> button_cancel_;

    std::unique_ptr<sfm::RectangleShape> shape_ = std::make_unique<sfm::RectangleShape>();
    std::unique_ptr<sfm::RectangleShape> normal_ = std::make_unique<sfm::RectangleShape>();
    std::unique_ptr<sfm::RectangleShape> onHover_ = std::make_unique<sfm::RectangleShape>();
    std::unique_ptr<sfm::RectangleShape> pressed_ = std::make_unique<sfm::RectangleShape>();
    std::unique_ptr<sfm::RectangleShape> released_ = std::make_unique<sfm::RectangleShape>();

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

    virtual ABarButton::State getOkState() const;
    virtual ABarButton::State getCancelState() const;
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


#endif // PLUGINS_CONTROL_PANEL
