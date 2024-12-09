#ifndef PHOTOSHOP_BAR_IMPLEMENTATION
#define PHOTOSHOP_BAR_IMPLEMENTATION


#include <api/api_bar.hpp>
#include <list>
#include <api/api_sfm.hpp>
#include <api_impl/actions.hpp>
#include <api_impl/sfm.hpp>


using namespace psapi;



class ABarButton : public IBarButton
{
protected:
    std::unique_ptr<sfm::Texture> texture_;
    std::unique_ptr<sfm::Sprite> sprite_;
    State state_;

    wid_t id_;

    const IBar *parent_;

    friend class BarButtonAction;
public:
    ABarButton( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);
    ~ABarButton() = default;

    virtual void draw(IRenderWindow* renderWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    virtual wid_t getId() const override;
    virtual IWindow* getWindowById(wid_t id) override;
    virtual const IWindow* getWindowById(wid_t id) const override;
    virtual vec2i getPos() const override;
    virtual vec2u getSize() const override;

    virtual void setParent(const IWindow* parent) override;
    virtual void setSize(const vec2u &size) override;
    virtual void setPos(const vec2i &pos) override;

    virtual void forceActivate() override;
    virtual void forceDeactivate() override;

    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

    virtual void setState(State state) override;
    virtual State getState() const override;
};


class AMenuButton : public IMenuButton
{
protected:
    std::unique_ptr<sfm::IRectangleShape> main_shape_;
    State state_ = IBarButton::State::Normal;

    wid_t id_;
    std::vector<std::unique_ptr<IWindow>> options_;

    std::unique_ptr<IBar> bar_;
    bool is_bar_active_ = false;

    const IBar *parent_ = nullptr;

    friend class AMenuButtonAction;
public:
    AMenuButton( wid_t init_id, std::unique_ptr<sfm::IRectangleShape> init_shape, std::unique_ptr<IBar> nested_bar);
    virtual ~AMenuButton() = default;

    virtual void addMenuItem(std::unique_ptr<IWindow> option) override;

    virtual void activateMenu() override;
    virtual void deactivateMenu() override;

    virtual IBar *getMenu() override;
    virtual const IBar *getMenu() const override;

    virtual void draw(IRenderWindow* renderWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    virtual wid_t getId() const override;
    virtual IWindow* getWindowById(wid_t id) override;
    virtual const IWindow* getWindowById(wid_t id) const override;
    virtual vec2i getPos() const override;
    virtual vec2u getSize() const override;

    virtual void setParent(const IWindow* parent) override;
    virtual void setSize(const vec2u &size) override;
    virtual void setPos(const vec2i &pos) override;

    virtual void forceActivate() override;
    virtual void forceDeactivate() override;

    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

    virtual void setState(State state) override;
    virtual State getState() const override;
};


class ABar : public IBar
{
protected:
    wid_t id_;

    vec2u size_;
    vec2i pos_;
    vec2u buttons_size_;

    bool is_active_;

    const IWindow *parent_;

    std::list<std::unique_ptr<IBarButton>> buttons_;

    std::unique_ptr<sfm::RectangleShape> main_shape_;
    std::unique_ptr<sfm::RectangleShape> normal_;
    std::unique_ptr<sfm::RectangleShape> onHover_;
    std::unique_ptr<sfm::RectangleShape> pressed_;
    std::unique_ptr<sfm::RectangleShape> released_;

    wid_t last_pressed_id_ = -1;

    friend class BarAction;
public:
    ABar( wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape,  std::unique_ptr<sfm::RectangleShape> &normal,
                                                            std::unique_ptr<sfm::RectangleShape> &onHover,
                                                            std::unique_ptr<sfm::RectangleShape> &pressed,
                                                            std::unique_ptr<sfm::RectangleShape> &released);
    virtual ~ABar() = default;

    virtual void draw(IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    virtual wid_t getId() const override;
    virtual IWindow* getWindowById(wid_t id) override;
    virtual const IWindow* getWindowById(wid_t id) const override;
    virtual vec2i getPos() const override;
    virtual vec2u getSize() const override;

    virtual void setParent(const IWindow* parent) override;
    virtual void setSize(const vec2u &size) override;
    virtual void setPos(const vec2i &pos) override;

    virtual void forceActivate() override;
    virtual void forceDeactivate() override;

    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

    virtual void addWindow(std::unique_ptr<IWindow> window) override;
    virtual void removeWindow(wid_t id) override;

    virtual bool unPressAllButtons() override;
    virtual void finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const override;

    virtual vec2i calculateNextPos(vec2i init_pos);
};


class ColorPalette : public IColorPalette
{
    sfm::Color color_ = sfm::Color(0, 0, 0);
public:
    sfm::Color getColor() const override;
    void setColor(const sfm::Color &color) override;
};


class ThicknessOption : public IThicknessOption
{
    float thickness_ = 0;
public:
    float getThickness() const override;
    void setThickness(float thickness) override;
};


class OpacityOption : public IOpacityOption
{
    float opacity_ = 0;
public:
    float getOpacity() const override;
    void setOpacity(float opacity) override;
};


class AOptionsBar : public IOptionsBar
{
protected:
    wid_t id_ = kOptionsBarWindowId;

    std::unique_ptr<sfm::RectangleShape> main_shape_;
    // std::unique_ptr<sfm::RectangleShape> normal_;
    // std::unique_ptr<sfm::RectangleShape> released_;
    // std::unique_ptr<sfm::RectangleShape> onHover_;
    // std::unique_ptr<sfm::RectangleShape> pressed_;
    vec2u size_;
    vec2i pos_;
    vec2u option_size_;

    bool is_active_ = false;

    const IWindow *parent_ = nullptr;

    std::list<std::unique_ptr<IBarButton>> options_;
public:
    AOptionsBar(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape);
    virtual ~AOptionsBar() = default;

    void draw(IRenderWindow* renderWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    wid_t getId() const override;
    IWindow* getWindowById(wid_t id) override;
    const IWindow* getWindowById(wid_t id) const override;
    vec2i getPos() const override;
    vec2u getSize() const override;

    void setParent(const IWindow* parent) override;
    void setSize(const vec2u &size) override;
    void setPos(const vec2i &pos) override;

    void forceActivate() override;
    void forceDeactivate() override;

    bool isActive() const override;
    bool isWindowContainer() const override;

    void addWindow(std::unique_ptr<IWindow> window) override;
    void removeWindow(wid_t id) override;

    void removeAllOptions() override;

    virtual vec2i calculateNextPos(vec2i init_pos);
};


class BarAction : public AAction
{
    ABar *bar_;
public:
    BarAction(ABar *bar, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


class BarButtonAction : public AAction
{
    ABarButton *button_;
public:
    BarButtonAction(ABarButton *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


class AMenuButtonAction : public AAction
{
    AMenuButton *button_;
public:
    AMenuButtonAction(AMenuButton *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


class AOptionsBarAction : public AAction
{
    AOptionsBar *bar_;
public:
    AOptionsBarAction(AOptionsBar *bar, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


#endif // PHOTOSHOP_BAR_IMPLEMENTATION
