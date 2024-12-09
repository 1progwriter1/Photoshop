#ifndef PLUGIN_MENU_ITEMS
#define PLUGIN_MENU_ITEMS


#include "api_impl/bar.hpp"


class MenuButton : public AMenuButton
{
    std::unique_ptr<sfm::IFont> font_;
    std::unique_ptr<sfm::IText> text_;
public:
    MenuButton(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape, std::unique_ptr<IBar> nested_bar);

    void draw(IRenderWindow *renderWindow) override;
    void setSize(const sfm::vec2u &size) override;
    void setPos(const sfm::vec2i &pos) override;

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;
    static std::unique_ptr<MenuButton> createMenuButton(wid_t id, const std::string &text);

    void updateState(const IRenderWindow *renderWindow, const Event &event);
private:
    sfm::IntRect fitTextToHeight(sf::Text& text, sfm::vec2i position, int height);
};




class MenuButtonAction : public AAction
{
    MenuButton *button_;
public:
    MenuButtonAction(MenuButton *button, const IRenderWindow *renderWindow, const Event &event);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;
};


class NestedMenu : public ABar
{
public:
    NestedMenu(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape,  std::unique_ptr<sfm::RectangleShape> &normal,
                                                            std::unique_ptr<sfm::RectangleShape> &onHover,
                                                            std::unique_ptr<sfm::RectangleShape> &pressed,
                                                            std::unique_ptr<sfm::RectangleShape> &released);
    static std::unique_ptr<NestedMenu> createMenuBar();
};



#endif // PLUGIN_MENU_ITEMS
