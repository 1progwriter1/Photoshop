#ifndef PLUGIN_MENU_ITEMS
#define PLUGIN_MENU_ITEMS


#include "api_impl/bar/bar.hpp"
#include "api_impl/bar/button.hpp"


#define FONT_FILE "../assets/fonts/Normal.ttf"
#define NESTED_BAR_COLOR sfm::Color( 128, 128, 128)


int getCharacterSize();


class MenuButton : public AMenuButton
{
    std::unique_ptr<sfm::IFont> font_;
    std::unique_ptr<sfm::IText> text_;
public:
    MenuButton(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape, std::unique_ptr<IBar> nested_bar);
    virtual ~MenuButton() = default;

    void draw(IRenderWindow *renderWindow) override;
    void setSize(const sfm::vec2u &size) override;
    virtual void setPos(const sfm::vec2i &pos) override;

    static std::unique_ptr<MenuButton> createMenuButton(wid_t id, const std::string &text);
};


class NestedMenu : public ABarShapes
{
    unsigned int min_size = 125;
    unsigned int min_offset = 25;
public:
    NestedMenu(wid_t init_id, std::unique_ptr<sfm::RectangleShape> main_shape, BarRectangleShapes &shapes);

    static std::unique_ptr<NestedMenu> createMenuBar(wid_t init_id);
    void setPos(const sfm::vec2i &pos) override;
    void addWindow(std::unique_ptr<IWindow> window) override;

    void finishButtonDraw(IRenderWindow *renderWindow, const IBarButton *button) const override;
};


class NestedMenuButton : public MenuButton
{
public:
    NestedMenuButton(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape, std::unique_ptr<IBar> nested_bar);

    void setPos(const sfm::vec2i &pos) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);

    static std::unique_ptr<NestedMenuButton> createNestedMenuButton(wid_t id, const std::string &text, IBar *parent);
};


class NestedMenuButtonAction : public AAction
{
    NestedMenuButton *button_;
public:
    NestedMenuButtonAction(NestedMenuButton *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
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
