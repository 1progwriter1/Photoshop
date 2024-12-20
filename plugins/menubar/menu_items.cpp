#include "menu_items.hpp"
#include <cassert>
#include <iostream>


int getCharacterSize()
{
    return 20;
}


std::unique_ptr<MenuButton> MenuButton::createMenuButton(wid_t id, const std::string &text)
{
    std::unique_ptr<sfm::IFont> font = sfm::IFont::create();
    font->loadFromFile(FONT_FILE);

    std::unique_ptr<sfm::IText> button_text = sfm::IText::create();
    button_text->setFont(font.get());
    sfm::Color color = sfm::Color::getStandardColor(psapi::sfm::Color::Type::White);
    button_text->setFillColor(&color);
    button_text->setString(text);
    sfm::IntRect text_rect = button_text->getGlobalBounds();

    std::unique_ptr<sfm::IRectangleShape> shape = sfm::RectangleShape::create(10, 10); // default size, it is not used later
    shape->setFillColor(sfm::Color::getStandardColor(psapi::sfm::Color::Type::Transparent));

    std::unique_ptr<IBar> bar = NestedMenu::createMenuBar(id);
    bar->setSize(sfm::vec2u(100, 300));
    return std::make_unique<MenuButton>(id, std::move(font), std::move(button_text), std::move(shape), std::move(bar));
}


std::unique_ptr<NestedMenu> NestedMenu::createMenuBar(wid_t init_id)
{
    std::unique_ptr<sfm::RectangleShape> main = std::make_unique<sfm::RectangleShape>();
    main->setFillColor(NESTED_BAR_COLOR);
    main->setOutlineThickness(1);
    main->setOutlineColor(sfm::Color(64, 64, 64));

    std::unique_ptr<sfm::RectangleShape> normal = std::make_unique<sfm::RectangleShape>();
    normal->setFillColor(sfm::Color(128, 128, 128));

    std::unique_ptr<sfm::RectangleShape> onHover = std::make_unique<sfm::RectangleShape>();
    onHover->setFillColor(sfm::Color(64, 64, 64));

    std::unique_ptr<sfm::RectangleShape> pressed = std::make_unique<sfm::RectangleShape>();
    pressed->setFillColor(sfm::Color(51, 255, 153));

    std::unique_ptr<sfm::RectangleShape> released = std::make_unique<sfm::RectangleShape>();
    released->setFillColor(sfm::Color(128, 128, 128));

    BarRectangleShapes shapes;
    shapes.normal = std::move( normal);
    shapes.on_hover = std::move( onHover);
    shapes.pressed = std::move( pressed);
    shapes.released = std::move( released);

    std::unique_ptr<NestedMenu> bar = std::make_unique<NestedMenu>( init_id, std::move(main), shapes);

    return bar;
}


void NestedMenu::setPos(const sfm::vec2i &pos)
{
    vec2i diff = pos - getPos();
    ABar::setPos(pos);
    for ( auto &button : buttons_ )
    {
        button->setPos(button->getPos() + diff);
    }
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
    text_->setCharacterSize(getCharacterSize());
    vec2u text_size = text_->getGlobalBounds().size;
    text_size.y = size.y;
    text_size.x += 20;

    AMenuButton::setSize(text_size);
}


void MenuButton::setPos(const sfm::vec2i &pos)
{
    AMenuButton::setPos(pos);

    vec2u text_size = text_->getGlobalBounds().size;
    text_->setPos(vec2f(pos.x + (AMenuButton::getSize().x - text_size.x) / 2.f, pos.y + 2));
    bar_->setPos(pos + sfm::vec2i(0, 32));
}


MenuButton::MenuButton(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape, std::unique_ptr<IBar> nested_bar)
    :   AMenuButton(init_id, std::move(init_shape), std::move(nested_bar)), font_(std::move(font)), text_(std::move(text))
{
    vec2u text_size = text_->getGlobalBounds().size;
    text_size.y = AMenuButton::getSize().y;
    text_size.x += 20;
    AMenuButton::setSize(text_size);
}


NestedMenu::NestedMenu(wid_t init_id, std::unique_ptr<sfm::RectangleShape> main_shape,  BarRectangleShapes &shapes)
    :   ABarShapes(init_id, std::move(main_shape), shapes)
{}


void NestedMenu::finishButtonDraw(IRenderWindow *renderWindow, const IBarButton *button) const
{
    vec2i pos = button->getPos();
    vec2u size = button->getSize();
    size.x = size_.x;
    switch ( button->getState() )
    {
        case IBarButton::State::Normal:
            normal_->setPosition( pos);
            normal_->setSize(size);
            renderWindow->draw( normal_.get());
            break;
        case IBarButton::State::Hover:
            on_hover_->setPosition( pos);
            on_hover_->setSize(size);
            renderWindow->draw( on_hover_.get());
            break;
        case IBarButton::State::Press:
            pressed_->setPosition( pos);
            pressed_->setSize(size);
            renderWindow->draw( pressed_.get());
            break;
        case IBarButton::State::Released:
            released_->setPosition( pos);
            released_->setSize(size);
            renderWindow->draw( released_.get());
            break;
        default:
            assert( 0 && "Unreachable" );
    }
}


void NestedMenu::addWindow(std::unique_ptr<IWindow> window)
{
    sfm::vec2i window_pos = pos_ + sfm::vec2i(0, 32 * buttons_.size());

    window->setPos(window_pos);
    window->setSize(sfm::vec2u(100, 32));

    if ( buttons_.empty() )
    {
        size_.x = window->getSize().x;
    } else
    {
        size_.x = std::max(size_.x, window->getSize().x);
    }
    buttons_.push_back( std::unique_ptr<IBarButton>( static_cast<IBarButton *>( window.release())));
    size_.y = 32 * buttons_.size();
}


TextButton::TextButton(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                        std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   id_(init_id), font_(std::move(font)), text_(std::move(text)), shape_(std::move(init_shape))
{
    shape_->setSize(text_->getGlobalBounds().size + sfm::vec2u(20, 2));
    vec2u text_size = text_->getGlobalBounds().size;
}


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
    text_->setCharacterSize(getCharacterSize());
    vec2u text_size = text_->getGlobalBounds().size;
    text_size.y = size.y;
    text_size.x += 20;
    shape_->setSize(text_size);
}


void TextButton::setPos(const vec2i &pos)
{
    shape_->setPosition(pos);

    vec2u text_size = text_->getGlobalBounds().size;
    text_->setPos(vec2f(pos.x + (shape_->getSize().x - text_size.x) / 2.f, pos.y + 2));
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


NestedMenuButton::NestedMenuButton(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                                   std::unique_ptr<sfm::IRectangleShape> init_shape, std::unique_ptr<IBar> nested_bar)
    :   MenuButton(init_id, std::move(font), std::move(text), std::move(init_shape), std::move(nested_bar))
{}


void NestedMenuButton::setPos(const sfm::vec2i &pos)
{
    MenuButton::setPos(pos);
    assert( parent_ );
    assert( dynamic_cast<const NestedMenu *>(parent_));
    bar_->setPos(vec2i(parent_->getPos().x + parent_->getSize().x, getPos().y));
}


std::unique_ptr<IAction> NestedMenuButton::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<NestedMenuButtonAction>(this, renderWindow, event);
}


std::unique_ptr<NestedMenuButton> NestedMenuButton::createNestedMenuButton(wid_t id, const std::string &text, IBar *parent)
{
    assert( parent );

    std::unique_ptr<sfm::IFont> font = sfm::IFont::create();
    font->loadFromFile(FONT_FILE);

    std::unique_ptr<sfm::IText> button_text = sfm::IText::create();
    button_text->setFont(font.get());
    sfm::Color color = sfm::Color::getStandardColor(psapi::sfm::Color::Type::White);
    button_text->setFillColor(&color);
    button_text->setString(text);
    sfm::IntRect text_rect = button_text->getGlobalBounds();

    std::unique_ptr<sfm::IRectangleShape> shape = sfm::RectangleShape::create(10, 10); // default size, it is not used later
    shape->setFillColor(sfm::Color::getStandardColor(psapi::sfm::Color::Type::Transparent));

    std::unique_ptr<NestedMenu> bar = NestedMenu::createMenuBar(id);
    bar->setSize(vec2u(100, 300));
    std::unique_ptr<NestedMenuButton> button = std::make_unique<NestedMenuButton>(id, std::move(font), std::move(button_text), std::move(shape), std::move(bar));

    button->setParent(parent);
    return button;
}


NestedMenuButtonAction::NestedMenuButtonAction(NestedMenuButton *button, const IRenderWindow *renderWindow, const Event &event)
    : AAction(renderWindow, event), button_(button) {}


bool NestedMenuButtonAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);
    button_->setPos(button_->getPos());
    return true;
}


bool NestedMenuButtonAction::isUndoable(const Key &key)
{
    return false;
}


void NestedMenuButton::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(AMenuButton::createAction(renderWindow, event));
}
