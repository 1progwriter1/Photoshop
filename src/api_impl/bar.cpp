#include <api_impl/bar.hpp>
#include <api_impl/sfm.hpp>
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


bool ABarButton::update(const IRenderWindow* renderWindow, const Event& event)
{
    sfm::vec2i mouse_pos = sfm::Mouse::getPosition( renderWindow);
    sfm::vec2i button_pos = getPos();

    sfm::vec2u size = getSize();
    bool is_on_focus = button_pos.x <= mouse_pos.x && mouse_pos.x <= button_pos.x + size.x &&
                        button_pos.y <= mouse_pos.y && mouse_pos.y <= button_pos.y + size.y;
    if ( is_on_focus )
    {
        if ( event.type == sfm::Event::MouseButtonPressed )
        {
            state_ = (state_ != IBarButton::State::Press) ? IBarButton::State::Press : IBarButton::State::Released;
        } else if ( state_ != IBarButton::State::Press )
        {
            state_ = psapi::IBarButton::State::Hover;
        }
    } else if ( state_ == psapi::IBarButton::State::Hover || state_ == psapi::IBarButton::State::Released )
    {
        state_ = psapi::IBarButton::State::Normal;
    }

    return true;
}


wid_t ABarButton::getId() const
{
    return id_;
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


Bar::Bar( wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape, std::unique_ptr<sfm::RectangleShape> &normal,
                                                    std::unique_ptr<sfm::RectangleShape> &onHover,
                                                    std::unique_ptr<sfm::RectangleShape> &pressed,
                                                    std::unique_ptr<sfm::RectangleShape> &released)
    :   id_( init_id), size_( main_shape->getSize()), pos_( vec2i( main_shape->getPosition().x, main_shape->getPosition().x))
{
    main_shape_ = std::move( main_shape);
    normal_ = std::move( normal);
    onHover_ = std::move( onHover);
    pressed_ = std::move( pressed);
    released_ = std::move( released);
}


void Bar::draw(IRenderWindow* renderWindow)
{
    assert( renderWindow );

    renderWindow->draw( main_shape_.get());

    // if ( getId() == kOptionsBarWindowId )
    // {
    //     std::cerr << "Amount of buttons: " << buttons_.size() << std::endl;
    // }

    for ( const auto &button : buttons_ )
    {
        // if ( getId() == kOptionsBarWindowId )
        // {
        //     std::cerr << "Button to draw has id: " << button->getId() << std::endl;
        // }
        button->draw( renderWindow);
        finishButtonDraw( renderWindow, button.get());

    }
}


bool Bar::update(const IRenderWindow* renderWindow, const Event& event)
{
    for ( auto &button : buttons_ )
    {
        if ( !button->update( renderWindow, event) )
            return false;
        if ( button->getState() == psapi::IBarButton::State::Press && button->getId() != last_pressed_id_ )
        {
            IWindow *prev_button = this->getWindowById( last_pressed_id_);
            if ( prev_button )
            {
                IBarButton *button_ptr =  static_cast<IBarButton *>( prev_button);
                if ( button_ptr->getState() == psapi::IBarButton::State::Press )
                {
                    button_ptr->setState( psapi::IBarButton::State::Normal);
                }
            }
            last_pressed_id_ = button->getId();
        }
    }
    return true;
}


void Bar::addWindow(std::unique_ptr<IWindow> window)
{
    if ( getId() == kOptionsBarWindowId )
    {
        std::cerr << "Added button has id: " << window->getId() << std::endl;
    } else
    {
        std::cerr << "Added button not mine has id: " << window->getId() << std::endl;
    }
    buttons_.push_back(  std::unique_ptr<IBarButton>( dynamic_cast<IBarButton *>( window.release())));
}


void Bar::removeWindow(wid_t id)
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


wid_t Bar::getId() const
{
    return id_;
}


IWindow* Bar::getWindowById(wid_t id)
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


const IWindow* Bar::getWindowById(wid_t id) const
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


vec2i Bar::getPos() const
{
    return pos_;
}


vec2u Bar::getSize() const
{
    return size_;
}


void Bar::setParent(const IWindow* parent)
{
    parent_ = parent;
}


void Bar::forceActivate()
{
    is_active_ = true;
}


void Bar::forceDeactivate()
{
    is_active_ = false;
}


bool Bar::isActive() const
{
    return is_active_;
}


bool Bar::isWindowContainer() const
{
    return true;
}


ChildInfo Bar::getNextChildInfo() const
{
    assert( 0 && "Not implemented" );

    return ChildInfo();
}


void Bar::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const
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
