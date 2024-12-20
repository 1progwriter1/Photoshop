#include "windows_id.hpp"
#include <api_impl/bar/bar.hpp>
#include <cassert>
#include <iostream>


ABar::ABar( wid_t init_id, std::unique_ptr<sfm::IRectangleShape> main_shape)
    :   id_( init_id), size_( main_shape->getSize()), pos_( vec2i( main_shape->getPosition().x, main_shape->getPosition().y))
{
    pos_ = vec2i( main_shape->getPosition().x, main_shape->getPosition().y);
    size_ = main_shape->getSize();
    main_shape_ = std::move( main_shape);
}


void ABar::draw(IRenderWindow* renderWindow)
{
    assert( renderWindow );

    main_shape_->setSize(size_);
    renderWindow->draw( main_shape_.get());

    for ( const auto &button : buttons_ )
    {
        finishButtonDraw( renderWindow, button.get());
        button->draw( renderWindow);
    }
}


std::unique_ptr<IAction> ABar::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<ABarAction>(this, renderWindow, event);
}


void ABar::addWindow(std::unique_ptr<IWindow> window)
{
    window->setPos( vec2i());
    window->setSize( buttons_size_);
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
        IWindow *but = nullptr;
        but = button->getWindowById( id);
        if ( but )
            return but;
    }
    return nullptr;
}


const IWindow* ABar::getWindowById(wid_t id) const
{
    if ( this->getId() == id ) return this;

    for ( const auto &button : buttons_ )
    {
        IWindow *but = nullptr;
        but = button->getWindowById( id);
        if ( but )
            return but;
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


ABarTextures::ABarTextures(wid_t init_id, std::unique_ptr<sfm::IRectangleShape> main_shape, BarTextures &textures)
    :   ABar(init_id, std::move(main_shape)),
        texture_normal_(std::move(textures.texture_normal)), sprite_normal_(std::move(textures.sprite_normal)),
        texture_on_hover_(std::move(textures.texture_on_hover)), sprite_on_hover_(std::move(textures.sprite_on_hover)),
        texture_pressed_(std::move(textures.texture_pressed)), sprite_pressed_(std::move(textures.sprite_pressed)),
        texture_released_(std::move(textures.texture_released)), sprite_released_(std::move(textures.sprite_released)) {}


void ABarTextures::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const
{
    vec2i pos = button->getPos();
    switch ( button->getState() )
    {
        case IBarButton::State::Normal:
            sprite_normal_->setPosition( pos.x, pos.y);
            renderWindow->draw( sprite_normal_.get());
            break;
        case IBarButton::State::Hover:
            sprite_on_hover_->setPosition( pos.x, pos.y);
            renderWindow->draw( sprite_on_hover_.get());
            break;
        case IBarButton::State::Press:
            sprite_pressed_->setPosition( pos.x, pos.y);
            renderWindow->draw( sprite_pressed_.get());
            break;
        case IBarButton::State::Released:
            sprite_released_->setPosition( pos.x, pos.y);
            renderWindow->draw( sprite_released_.get());
            break;
        default:
            assert( 0 && "Unreachable" );

    }
}


ABarShapes::ABarShapes(wid_t init_id, std::unique_ptr<sfm::IRectangleShape> main_shape, BarRectangleShapes &shapes)
    :   ABar(init_id, std::move(main_shape)),
        normal_(std::move(shapes.normal)), on_hover_(std::move(shapes.on_hover)), pressed_(std::move(shapes.pressed)), released_(std::move(shapes.released)) {}


void ABarShapes::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const
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


AOptionsBar::AOptionsBar(wid_t init_id, std::unique_ptr<sfm::IRectangleShape> &main_shape)
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
    if ( this->getId() == id ) return this;

    for ( const auto &option : options_ )
    {
        IWindow *opt = nullptr;
        opt = option->getWindowById( id);
        if ( opt )
            return opt;
    }
    return nullptr;
}


IWindow* AOptionsBar::getWindowById(wid_t id)
{
    if ( this->getId() == id ) return this;

    for ( const auto &option : options_ )
    {
        IWindow *opt = nullptr;
        opt = option->getWindowById( id);
        if ( opt )
            return opt;
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
    sfm::IntRect rect = getOptionsBarIntRect();
    vec2u w_size = window->getSize();
    float ratio = w_size.x == 0 ? 1 : (float) w_size.y / w_size.x;

    w_size.x = rect.size.x - 10;
    w_size.y = rect.size.x * ratio;

    window->setSize( w_size);
    window->setPos( rect.pos + vec2i(5, 5));
    options_.push_back( std::move(window));
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
    for ( auto &option : bar_->options_ )
    {
        if ( !psapi::getActionController()->execute(option->createAction(render_window_, event_)) )
            return false;
    }
    return true;
}
