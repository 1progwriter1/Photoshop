#include "menu_items.hpp"
#include <cassert>
#include <iostream>


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

    std::unique_ptr<IBar> bar = NestedMenu::createMenuBar(id);
    bar->setSize(sfm::vec2u(100, 300));
    return std::make_unique<MenuButton>(id, std::move(font), std::move(button_text), std::move(shape), std::move(bar));
}


std::unique_ptr<NestedMenu> NestedMenu::createMenuBar(wid_t init_id)
{
    std::unique_ptr<sfm::RectangleShape> main = std::make_unique<sfm::RectangleShape>();
    main->setFillColor(NESTED_BAR_COLOR);

    std::unique_ptr<sfm::RectangleShape> normal = std::make_unique<sfm::RectangleShape>();
    normal->setFillColor(sfm::Color());

    std::unique_ptr<sfm::RectangleShape> onHover = std::make_unique<sfm::RectangleShape>();
    onHover->setFillColor(sfm::Color());
    onHover->setOutlineColor( sfm::Color::getStandardColor(psapi::sfm::Color::Type::White));
    onHover->setOutlineThickness( 5);

    std::unique_ptr<sfm::RectangleShape> pressed = std::make_unique<sfm::RectangleShape>();
    pressed->setFillColor(sfm::Color());
    pressed->setOutlineColor( sfm::Color::getStandardColor(psapi::sfm::Color::Type::Magenta));
    pressed->setOutlineThickness( 5);

    std::unique_ptr<sfm::RectangleShape> released = std::make_unique<sfm::RectangleShape>();
    released->setFillColor(sfm::Color());

    std::unique_ptr<NestedMenu> bar = std::make_unique<NestedMenu>( init_id, main,
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


NestedMenu::NestedMenu(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape,  std::unique_ptr<sfm::RectangleShape> &normal,
                                                            std::unique_ptr<sfm::RectangleShape> &onHover,
                                                            std::unique_ptr<sfm::RectangleShape> &pressed,
                                                            std::unique_ptr<sfm::RectangleShape> &released)
    :   ABar(init_id, main_shape, normal, onHover, pressed, released)
{}


void NestedMenu::addWindow(std::unique_ptr<IWindow> window)
{
    sfm::vec2i window_pos = pos_ + sfm::vec2i(10, 10 + 40 * buttons_.size());
    window->setPos(window_pos);
    window->setSize(sfm::vec2u(0, 30));
    if ( buttons_.empty() )
    {
        size_.x = window->getSize().x + 20;
    } else
    {
        size_.x = std::max(size_.x, window->getSize().x + 20);
    }
    buttons_.push_back( std::unique_ptr<IBarButton>( static_cast<IBarButton *>( window.release())));
    size_.y = 10 + 40 * buttons_.size();
}


TextButton::TextButton(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                        std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   id_(init_id), font_(std::move(font)), text_(std::move(text)), shape_(std::move(init_shape)) {}


void TextButton::draw(IRenderWindow* renderWindow)
{
    assert( renderWindow );

    renderWindow->draw( shape_.get());
    renderWindow->draw( text_.get());
}


std::unique_ptr<IAction> TextButton::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<TextButtonAction>(this, renderWindow, event);
}


wid_t TextButton::getId() const
{
    return id_;
}


IWindow* TextButton::getWindowById(wid_t id)
{
    if ( id == id_ )
        return this;

    return nullptr;
}


const IWindow* TextButton::getWindowById(wid_t id) const
{
    if ( id == id_ )
        return this;

    return nullptr;
}


vec2i TextButton::getPos() const
{
    return vec2i(shape_->getPosition().x, shape_->getPosition().y);
}


vec2u TextButton::getSize() const
{
    return shape_->getSize();
}


void TextButton::setParent(const IWindow* parent)
{
    parent_ = dynamic_cast<const IBar *>( parent);
    assert( parent_ && "parent is not IBar" );
}


void TextButton::setSize(const vec2u &size)
{
    sfm::Text *text = dynamic_cast<sfm::Text *>(text_.get());
    assert( text );

    sfm::IntRect button_rect = fitTextToHeight(text->getText(), getPos(), size.y);

    shape_->setSize(button_rect.size);
}


void TextButton::setPos(const vec2i &pos)
{
    shape_->setPosition(pos);

    sfm::Text *text = dynamic_cast<sfm::Text *>(text_.get());
    assert( text );

    sfm::IntRect button_rect = fitTextToHeight(text->getText(), getPos(), getSize().y);

    shape_->setSize(button_rect.size);
}


void TextButton::forceActivate()
{
    assert( 0 && "Not implemented" );
}


void TextButton::forceDeactivate()
{
    assert( 0 && "Not implemented" );
}


bool TextButton::isActive() const
{
    assert( 0 && "Not implemented" );
    return true;
}


bool TextButton::isWindowContainer() const
{
    return false;
}


void TextButton::setState(State state)
{
    state_ = state;
}


IBarButton::State TextButton::getState() const
{
    return state_;
}


TextButtonAction::TextButtonAction(TextButton *button, const IRenderWindow *renderWindow, const Event &event)
    : AAction(renderWindow, event), button_(button) {}


bool TextButtonAction::isUndoable(const Key &key)
{
    return false;
}


bool TextButtonAction::execute(const Key &key)
{
    sfm::vec2i mouse_pos = sfm::Mouse::getPosition(render_window_);
    sfm::vec2i button_pos = button_->getPos();

    sfm::vec2u size = button_->getSize();
    bool is_on_focus = button_pos.x <= mouse_pos.x && mouse_pos.x <= button_pos.x + size.x &&
                        button_pos.y <= mouse_pos.y && mouse_pos.y <= button_pos.y + size.y;
    if ( is_on_focus )
    {
        if ( event_.type == sfm::Event::MouseButtonPressed )
        {
            button_->state_ = (button_->state_ != IBarButton::State::Press) ? IBarButton::State::Press : IBarButton::State::Released;
        } else if ( button_->state_ != IBarButton::State::Press )
        {
            button_->state_ = psapi::IBarButton::State::Hover;
        }
    } else if ( button_->state_ == psapi::IBarButton::State::Hover || button_->state_ == psapi::IBarButton::State::Released )
    {
        button_->state_ = psapi::IBarButton::State::Normal;
    }
    return true;
}


sfm::IntRect fitTextToHeight(sf::Text& text, sfm::vec2i position, int height)
{

    float padding = height * 0.15f;
    float availableHeight = height - 2 * padding;

    unsigned int characterSize = height;
    text.setCharacterSize(characterSize);

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
