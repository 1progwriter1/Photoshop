#include "windows_id.hpp"
#include <api_impl/bar/bar.hpp>
#include <cassert>


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

    main_shape_->setSize(size_);
    renderWindow->draw( main_shape_.get());

    for ( const auto &button : buttons_ )
    {
        button->draw( renderWindow);
        finishButtonDraw( renderWindow, button.get());
    }
}


std::unique_ptr<IAction> ABar::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<ABarAction>(this, renderWindow, event);
}


void ABar::addWindow(std::unique_ptr<IWindow> window)
{
    window->setPos( vec2i());
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
    vec2u size = button->getSize();
    switch ( button->getState() )
    {
        case IBarButton::State::Normal:
            normal_->setPosition( pos);
            normal_->setSize(size);
            renderWindow->draw( normal_.get());
            break;
        case IBarButton::State::Hover:
            onHover_->setPosition( pos);
            onHover_->setSize(size);
            renderWindow->draw( onHover_.get());
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


ABarAction::ABarAction(ABar *bar, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), bar_(bar) {}



bool ABarAction::isUndoable(const Key &key)
{
    return false;
}


bool ABarAction::execute(const Key &key)
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
