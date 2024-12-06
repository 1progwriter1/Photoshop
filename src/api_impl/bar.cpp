#include <api_impl/bar.hpp>
#include <cassert>
#include <iostream>


void ABarButton::draw(IRenderWindow* renderWindow)
{
    renderWindow->draw( sprite_.get());
}


ABarButton::ABarButton(  wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   id_( init_id)
{
    texture_ = std::move( init_texture);
    sprite_ = std::move( init_sprite);
}


std::unique_ptr<IAction> ABarButton::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<BarButtonAction>(this, renderWindow, event);
}


wid_t ABarButton::getId() const
{
    return id_;
}


void ABarButton::setSize(const vec2u &size)
{
    sprite_->setTextureRect(sfm::IntRect(sfm::vec2dToVec2u(sprite_->getPosition()), size));
}


void ABarButton::setPos(const vec2i &pos)
{
    sprite_->setPosition( pos.x, pos.y);
}


IWindow* ABarButton::getWindowById(wid_t id)
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


const IWindow* ABarButton::getWindowById(wid_t id) const
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


vec2i ABarButton::getPos() const
{
    vec2f pos = sprite_->getPosition();
    return vec2i( static_cast<int>( pos.x), static_cast<int>( pos.y));
}


vec2u ABarButton::getSize() const
{
    return sprite_->getSize();
}


void ABarButton::setParent(const IWindow* parent)
{
    parent_ = dynamic_cast<const IBar *>( parent);
    assert( parent_ );
}


void ABarButton::forceActivate()
{
    assert( 0 && "Not implemented" );
}


void ABarButton::forceDeactivate()
{
    assert( 0 && "Not implemented" );
}


bool ABarButton::isActive() const
{
    assert( 0 && "Not implemented" );

    return false;
}


bool ABarButton::isWindowContainer() const
{
    assert( 0 && "Not implemented" );

    return false;
}


void ABarButton::setState(State state)
{
    state_ = state;
}


IBarButton::State ABarButton::getState() const
{
    return state_;
}


AMenuButton::AMenuButton(wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite, std::unique_ptr<IBar> &nested_bar)
    :   id_(init_id), texture_(std::move(init_texture)), sprite_(std::move(init_sprite)) {}


void AMenuButton::addMenuItem(std::unique_ptr<IWindow> option)
{
    assert( option );

    options_.push_back( std::move( option));
}


void AMenuButton::activateMenu()
{
    is_bar_active_ = true;
}


void AMenuButton::deactivateMenu()
{
    is_bar_active_ = false;
}


IBar *AMenuButton::getMenu()
{
    return bar_.get();
}


const IBar *AMenuButton::getMenu() const
{
    return bar_.get();
}


void AMenuButton::draw(IRenderWindow* renderWindow)
{
    renderWindow->draw( sprite_.get());
}


std::unique_ptr<IAction> AMenuButton::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


wid_t AMenuButton::getId() const
{
    return id_;
}


IWindow* AMenuButton::getWindowById(wid_t id)
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


const IWindow* AMenuButton::getWindowById(wid_t id) const
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


vec2i AMenuButton::getPos() const
{
    vec2f pos = sprite_->getPosition();
    return vec2i( static_cast<int>( pos.x), static_cast<int>( pos.y));
}


vec2u AMenuButton::getSize() const
{
    return sprite_->getSize();
}


void AMenuButton::setParent(const IWindow* parent)
{
    parent_ = dynamic_cast<const IBar *>( parent);
    assert( parent_ );
}


void AMenuButton::setPos(const vec2i &pos)
{
    sprite_->setPosition( pos.x, pos.y);
}


void AMenuButton::setSize(const vec2u &size)
{
    assert( 0 && "Not implemented" );
}


void AMenuButton::forceActivate()
{
    assert( 0 && "Not implemented" );
}


void AMenuButton::forceDeactivate()
{
    assert( 0 && "Not implemented" );
}


bool AMenuButton::isActive() const
{
    assert( 0 && "Not implemented" );

    return false;
}


bool AMenuButton::isWindowContainer() const
{
    return false;
}


void AMenuButton::setState(State state)
{
    state_ = state;
}


IBarButton::State AMenuButton::getState() const
{
    return state_;
}


ABar::ABar( wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape, std::unique_ptr<sfm::RectangleShape> &normal,
                                                    std::unique_ptr<sfm::RectangleShape> &onHover,
                                                    std::unique_ptr<sfm::RectangleShape> &pressed,
                                                    std::unique_ptr<sfm::RectangleShape> &released)
    :   id_( init_id), size_( main_shape->getSize()), pos_( vec2i( main_shape->getPosition().x, main_shape->getPosition().y)),
    buttons_size_(normal->getSize())
{
    assert( buttons_size_.x == released->getSize().x );
    assert( buttons_size_.y == released->getSize().y );

    assert( buttons_size_.x == pressed->getSize().x );
    assert( buttons_size_.y == pressed->getSize().y );

    assert( buttons_size_.x == onHover->getSize().x );
    assert( buttons_size_.y == onHover->getSize().y );

    main_shape_ = std::move( main_shape);
    normal_ = std::move( normal);
    onHover_ = std::move( onHover);
    pressed_ = std::move( pressed);
    released_ = std::move( released);
}


void ABar::draw(IRenderWindow* renderWindow)
{
    assert( renderWindow );

    renderWindow->draw( main_shape_.get());

    for ( const auto &button : buttons_ )
    {
        button->draw( renderWindow);
        finishButtonDraw( renderWindow, button.get());

    }
}


std::unique_ptr<IAction> ABar::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<BarAction>(this, renderWindow, event);
}


void ABar::addWindow(std::unique_ptr<IWindow> window)
{
    window->setSize(buttons_size_);
    vec2i pos = calculateNextPos(window->getPos());
    window->setPos(pos);
    buttons_.push_back( std::unique_ptr<IBarButton>( dynamic_cast<IBarButton *>( window.release())));
}


void ABar::removeWindow(wid_t id)
{
    auto iter = buttons_.begin();
    for ( auto &button : buttons_ )
    {
        if ( button->getId() == id )
        {
            buttons_.erase( iter);
            break;
        }
        iter++;
    }
}


wid_t ABar::getId() const
{
    return id_;
}


bool ABar::unPressAllButtons()
{
    for ( auto &button : buttons_ )
    {
        button->setState( IBarButton::State::Normal);
    }
    return true;
}


void ABar::setPos(const vec2i &pos)
{
    pos_ = pos;
    main_shape_->setPosition( pos);
}


void ABar::setSize(const vec2u &size)
{
    size_ = size;
    main_shape_->setSize( size);
}


IWindow* ABar::getWindowById(wid_t id)
{

    if ( this->getId() == id ) return this;

    for ( const auto &button : buttons_ )
    {
        if ( button->getId() == id )
        {
            return button.get();
        }
    }
    return nullptr;
}


const IWindow* ABar::getWindowById(wid_t id) const
{
    if ( id == this->getId() ) return this;

    for ( const auto &button : buttons_ )
    {
        if ( button->getId() == id )
        {
            return button.get();
        }
    }
    return nullptr;
}


vec2i ABar::getPos() const
{
    return pos_;
}


vec2u ABar::getSize() const
{
    return size_;
}


void ABar::setParent(const IWindow* parent)
{
    parent_ = parent;
}


void ABar::forceActivate()
{
    is_active_ = true;
}


vec2i ABar::calculateNextPos(vec2i init_pos)
{
    return init_pos;
}


void ABar::forceDeactivate()
{
    is_active_ = false;
}


bool ABar::isActive() const
{
    return is_active_;
}


bool ABar::isWindowContainer() const
{
    return true;
}


void ABar::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const
{
    vec2i pos = button->getPos();
    switch ( button->getState() )
    {
        case IBarButton::State::Normal:
            normal_->setPosition( pos);
            renderWindow->draw( normal_.get());
            break;
        case IBarButton::State::Hover:
            onHover_->setPosition( pos);
            renderWindow->draw( onHover_.get());
            break;
        case IBarButton::State::Press:
            pressed_->setPosition( pos);
            renderWindow->draw( pressed_.get());
            break;
        case IBarButton::State::Released:
            released_->setPosition( pos);
            renderWindow->draw( released_.get());
            break;
        default:
            assert( 0 && "Unreachable" );

    }
}


sfm::Color ColorPalette::getColor() const
{
    return color_;
}


void ColorPalette::setColor(const sfm::Color &color)
{
    color_ = color;
}


float ThicknessOption::getThickness() const
{
    return thickness_;
}


void ThicknessOption::setThickness(float thickness)
{
    thickness_ = thickness;
}


float OpacityOption::getOpacity() const
{
    return opacity_;
}


void OpacityOption::setOpacity(float opacity)
{
    opacity_ = opacity;
}


AOptionsBar::AOptionsBar(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape)
    :   id_( init_id), main_shape_(std::move(main_shape))
        // size_( main_shape->getSize()),
        // pos_( vec2i( main_shape->getPosition().x, main_shape->getPosition().y)),
        // option_size_(main_shape->getSize())
{}


void AOptionsBar::draw(IRenderWindow* renderWindow)
{
    assert( renderWindow );

    renderWindow->draw( main_shape_.get());

    for ( const auto &option : options_ )
    {
        option->draw( renderWindow);
    }
}


std::unique_ptr<IAction> AOptionsBar::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<AOptionsBarAction>(this, renderWindow, event);
}


wid_t AOptionsBar::getId() const
{
    return id_;
}


const IWindow* AOptionsBar::getWindowById(wid_t id) const
{
    for ( const auto &option : options_ )
    {
        if ( option->getId() == id )
        {
            return option.get();
        }
    }
    return nullptr;
}


IWindow* AOptionsBar::getWindowById(wid_t id)
{
    if ( id == id_ )
        return this;
    for ( auto &option : options_ )
    {
        if ( option->getId() == id )
        {
            return option.get();
        }
    }
    return nullptr;
}


vec2i AOptionsBar::getPos() const
{
    return pos_;
}


vec2u AOptionsBar::getSize() const
{
    return size_;
}


void AOptionsBar::setParent(const IWindow* parent)
{
    parent_ = parent;
}


void AOptionsBar::setPos(const vec2i &pos)
{
    pos_ = pos;

    main_shape_->setPosition( pos);
}


void AOptionsBar::setSize(const vec2u &size)
{
    size_ = size;

    main_shape_->setSize( size);
}


void AOptionsBar::forceActivate()
{
    is_active_ = true;
}


void AOptionsBar::forceDeactivate()
{
    is_active_ = false;
}


bool AOptionsBar::isActive() const
{
    return is_active_;
}


bool AOptionsBar::isWindowContainer() const
{
    return true;
}


vec2i AOptionsBar::calculateNextPos(vec2i init_pos)
{
    return init_pos;
}


void AOptionsBar::addWindow(std::unique_ptr<IWindow> window)
{
    IBarButton *button = dynamic_cast<IBarButton *>( window.release());
    assert( button );

    options_.push_back( std::unique_ptr<IBarButton>( button));
}


void AOptionsBar::removeWindow(wid_t id)
{
    for ( auto it = options_.begin(); it != options_.end(); ++it )
    {
        if ( (*it)->getId() == id )
        {
            options_.erase( it);
            return;
        }
    }
}


void AOptionsBar::removeAllOptions()
{
    options_.clear();
}


BarAction::BarAction(ABar *bar, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), bar_(bar) {}



bool BarAction::isUndoable(const Key &key)
{
    return false;
}


bool BarAction::execute(const Key &key)
{
    for ( auto &button : bar_->buttons_ )
    {
        if ( !psapi::getActionController()->execute(button->createAction(render_window_, event_)) )
            return false;

        if ( button->getState() == psapi::IBarButton::State::Press && button->getId() != bar_->last_pressed_id_ )
        {
            IWindow *prev_button = bar_->getWindowById( bar_->last_pressed_id_);
            if ( prev_button )
            {
                IBarButton *button_ptr =  static_cast<IBarButton *>( prev_button);
                if ( button_ptr->getState() == psapi::IBarButton::State::Press )
                {
                    button_ptr->setState( psapi::IBarButton::State::Normal);
                }
            }
            bar_->last_pressed_id_ = button->getId();
        }
    }
    return true;
}


BarButtonAction::BarButtonAction(ABarButton *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button) {}


bool BarButtonAction::isUndoable(const Key &key)
{
    return false;
}


bool BarButtonAction::execute(const Key &key)
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


AOptionsBarAction::AOptionsBarAction(AOptionsBar *bar, const IRenderWindow *render_window, const Event &event)
    :   AAction(render_window, event), bar_(bar) {}


bool AOptionsBarAction::isUndoable(const Key &key)
{
    return false;
}


bool AOptionsBarAction::execute(const Key &key)
{
    return true;
}
