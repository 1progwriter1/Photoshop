#include "menu_items.hpp"
#include "api_impl/bar.hpp"
#include <cassert>


const char *FONT_FILE = "../assets/fonts/HomeplanetbbBolditalic.ttf";
const sfm::Color NESTED_BAR_COLOR = sfm::Color( 224, 224, 224);


std::unique_ptr<MenuButton> MenuButton::createMenuButton(wid_t id, const std::string &text)
{
    std::unique_ptr<sfm::IFont> font = sfm::IFont::create();
    font->loadFromFile(FONT_FILE);

    std::unique_ptr<sfm::IText> button_text = sfm::IText::create();
    button_text->setFont(font.get());
    sfm::Color color = sfm::Color::getStandardColor(psapi::sfm::Color::Type::Black);
    button_text->setFillColor(&color);
    button_text->setString(text);
    sfm::IntRect text_rect = button_text->getGlobalBounds();

    std::unique_ptr<sfm::IRectangleShape> shape = sfm::RectangleShape::create(10, 10); // default size, it is not used later
    shape->setFillColor(sfm::Color::getStandardColor(psapi::sfm::Color::Type::White));

    std::unique_ptr<IBar> bar = NestedMenu::createMenuBar();
    bar->setSize(sfm::vec2u(100, 300));
    return std::make_unique<MenuButton>(id, std::move(font), std::move(button_text), std::move(shape), std::move(bar));
}


std::unique_ptr<NestedMenu> NestedMenu::createMenuBar()
{
    std::unique_ptr<sfm::RectangleShape> main = std::make_unique<sfm::RectangleShape>();
    main->setFillColor( NESTED_BAR_COLOR);

    std::unique_ptr<sfm::RectangleShape> normal = std::make_unique<sfm::RectangleShape>();
    normal->setFillColor( sfm::Color());

    std::unique_ptr<sfm::RectangleShape> onHover = std::make_unique<sfm::RectangleShape>();
    onHover->setFillColor( sfm::Color(204, 229, 255));

    std::unique_ptr<sfm::RectangleShape> pressed = std::make_unique<sfm::RectangleShape>();
    pressed->setFillColor( sfm::Color(153, 204, 255));

    std::unique_ptr<sfm::RectangleShape> released = std::make_unique<sfm::RectangleShape>();
    released->setFillColor( sfm::Color());

    std::unique_ptr<NestedMenu> bar = std::make_unique<NestedMenu>( kToolBarWindowId, main,
                                                                normal,
                                                                onHover,
                                                                pressed,
                                                                released);

    return bar;
}


void MenuButton::draw(IRenderWindow *renderWindow)
{
    AMenuButton::draw(renderWindow);
    renderWindow->draw(text_.get());

    if ( is_bar_active_ )
    {
        bar_->draw(renderWindow);
    }
}

void MenuButton::setSize(const sfm::vec2u &size)
{

    AMenuButton::setSize(size);

    sfm::Text *text = dynamic_cast<sfm::Text *>(text_.get());
    assert( text );

    sfm::IntRect button_rect = fitTextToHeight(text->getText(), AMenuButton::getPos(), size.y);

    AMenuButton::setSize(button_rect.size);
}


void MenuButton::setPos(const sfm::vec2i &pos)
{
    AMenuButton::setPos(pos);

    sfm::Text *text = dynamic_cast<sfm::Text *>(text_.get());
    assert( text );

    sfm::IntRect button_rect = fitTextToHeight(text->getText(), AMenuButton::getPos(), AMenuButton::getSize().y);

    AMenuButton::setSize(button_rect.size);

    sfm::vec2i bar_pos = sfm::vec2i(button_rect.pos.x, button_rect.pos.y + button_rect.size.y);
    bar_->setPos(bar_pos + sfm::vec2i(0, 10));
}


MenuButton::MenuButton(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape, std::unique_ptr<IBar> nested_bar)
    :   AMenuButton(init_id, std::move(init_shape), std::move(nested_bar)), font_(std::move(font)), text_(std::move(text))
{
}


void MenuButton::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(AMenuButton::createAction(renderWindow, event));
}


std::unique_ptr<IAction> MenuButton::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<MenuButtonAction>(this, renderWindow, event);
}


MenuButtonAction::MenuButtonAction(MenuButton *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button) {}


bool MenuButtonAction::isUndoable(const Key &key)
{
    return false;
}


bool MenuButtonAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);
    return true;
}


NestedMenu::NestedMenu(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape,  std::unique_ptr<sfm::RectangleShape> &normal,
                                                            std::unique_ptr<sfm::RectangleShape> &onHover,
                                                            std::unique_ptr<sfm::RectangleShape> &pressed,
                                                            std::unique_ptr<sfm::RectangleShape> &released)
    :   ABar(init_id, main_shape, normal, onHover, pressed, released)
{}


sfm::IntRect MenuButton::fitTextToHeight(sf::Text& text, sfm::vec2i position, int height)
{

    float padding = height * 0.1f;
    float availableHeight = height - 2 * padding;

    unsigned int characterSize = text.getCharacterSize();

    sf::FloatRect textBounds = text.getLocalBounds();
    while ( textBounds.height > availableHeight && characterSize > 1 )
    {
        characterSize--;
        text.setCharacterSize(characterSize);
        textBounds = text.getLocalBounds();
    }

    float width = textBounds.width + 2 * padding;

    float x = position.x + padding - textBounds.left;
    float y = position.y + padding + (availableHeight - textBounds.height) / 2 - textBounds.top;

    text.setPosition(x, y);

    return sfm::IntRect({position.x, position.y}, sfm::vec2u(static_cast<int>(width), static_cast<int>(height)));
}
