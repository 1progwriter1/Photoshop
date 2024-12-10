#ifndef PLUGIN_MENU_ITEMS
#define PLUGIN_MENU_ITEMS


#include "api_impl/bar/bar.hpp"
#include "api_impl/bar/button.hpp"


#define FONT_FILE "../assets/fonts/HomeplanetbbBolditalic.ttf"
#define NESTED_BAR_COLOR sfm::Color( 224, 224, 224)


class MenuButton : public AMenuButton
{
    std::unique_ptr<sfm::IFont> font_;
    std::unique_ptr<sfm::IText> text_;

    friend class MenuButtonAction;
public:
    MenuButton(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape, std::unique_ptr<IBar> nested_bar);

    void draw(IRenderWindow *renderWindow) override;
    void setSize(const sfm::vec2u &size) override;
    void setPos(const sfm::vec2i &pos) override;

    static std::unique_ptr<MenuButton> createMenuButton(wid_t id, const std::string &text);
};


class NestedMenu : public ABar
{
public:
    NestedMenu(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape,  std::unique_ptr<sfm::RectangleShape> &normal,
                                                            std::unique_ptr<sfm::RectangleShape> &onHover,
                                                            std::unique_ptr<sfm::RectangleShape> &pressed,
                                                            std::unique_ptr<sfm::RectangleShape> &released);
    static std::unique_ptr<NestedMenu> createMenuBar(wid_t init_id);

    void addWindow(std::unique_ptr<IWindow> window) override;
};


class TextButton : public IBarButton
{
protected:
    std::unique_ptr<sfm::IFont> font_;
    std::unique_ptr<sfm::IText> text_;
    std::unique_ptr<sfm::IRectangleShape> shape_;

    wid_t id_;

    State state_;

    const IBar *parent_;

    friend class TextButtonAction;
public:
    TextButton(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);
    virtual ~TextButton() = default;

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

    virtual void setState(State state) override;
    virtual State getState() const override;
};


class TextButtonAction : public AAction
{
    TextButton *button_;
public:
    TextButtonAction(TextButton *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


sfm::IntRect fitTextToHeight(sf::Text& text, sfm::vec2i position, int height);


#endif // PLUGIN_MENU_ITEMS
