#include <api_impl/bar.hpp>
#include <cassert>
#include <iostream>


void ABarButton::draw(IRenderWindow* renderWindow)
{
    renderWindow->draw( sprite_.get());
}


ABarButton::ABarButton(  std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
{
    texture_ = std::move( init_texture);
    sprite_ = std::move( init_sprite);
}


bool ABarButton::update(const IRenderWindow* renderWindow, const Event& event)
{
    assert( 0 && "Not implemented" );

    return true;
}


wid_t ABarButton::getId() const
{
    assert( 0 && "Not implemented" );

    return 0;
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
    assert( 0 && "Not implemented" );

    return vec2u();
}


void ABarButton::setParent(const IWindow* parent)
{
    assert( 0 && "Not implemented" );
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
    assert( 0 && "Not implemented" );
}


IBarButton::State ABarButton::getState() const
{
    return IBarButton::State::Normal;
}


Bar::Bar( std::unique_ptr<sfm::RectangleShape> &main_shape, std::unique_ptr<sfm::RectangleShape> &normal,
                                                    std::unique_ptr<sfm::RectangleShape> &onHover,
                                                    std::unique_ptr<sfm::RectangleShape> &pressed,
                                                    std::unique_ptr<sfm::RectangleShape> &released)
    :   size_( main_shape->getSize()), pos_( vec2i( main_shape->getPosition().x, main_shape->getPosition().x))
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

    for ( auto &button : buttons_ )
    {
        button->draw( renderWindow);
        finishButtonDraw( renderWindow, button.get());

    }
}


bool Bar::update(const IRenderWindow* renderWindow, const Event& event)
{
    assert( 0 && "Not implemented" );

    return true;
}


void Bar::addWindow(std::unique_ptr<IWindow> window)
{
    buttons_.push_back(  std::unique_ptr<IBarButton>( dynamic_cast<IBarButton *>( window.release())));
    // buttons_.back()->draw( nullptr);
    // win = std::move( window);
    // button_ =  dynamic_cast<IBarButton *>( win.get());
}


void Bar::removeWindow(wid_t id)
{
    assert( 0 && "Not implemented" );
}


wid_t Bar::getId() const
{
    return kToolBarWindowId;
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
