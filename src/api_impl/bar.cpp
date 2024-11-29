#include <api_impl/bar.hpp>
#include <cassert>


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
    assert( 0 && "Not implemented" );

    return nullptr;
}


wid_t ABarButton::getId() const
{
    return id_;
}


void ABarButton::setSize(const vec2u &size)
{
    assert( 0 && "Not implemented" );
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


AOptionButton::AOptionButton(wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite, std::unique_ptr<IBar> &nested_bar)
    :   id_(init_id), texture_(std::move(init_texture)), sprite_(std::move(init_sprite)) {}


void AOptionButton::addOption(std::unique_ptr<IWindow> option)
{
    assert( option );

    options_.push_back( std::move( option));
}


void AOptionButton::activateBar()
{
    is_bar_active_ = true;
}


void AOptionButton::deactivateBar()
{
    is_bar_active_ = false;
}


IBar *AOptionButton::getBar()
{
    return bar_.get();
}


const IBar *AOptionButton::getBar() const
{
    return bar_.get();
}


void AOptionButton::draw(IRenderWindow* renderWindow)
{
    renderWindow->draw( sprite_.get());
}


std::unique_ptr<IAction> AOptionButton::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


wid_t AOptionButton::getId() const
{
    return id_;
}


IWindow* AOptionButton::getWindowById(wid_t id)
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


const IWindow* AOptionButton::getWindowById(wid_t id) const
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


vec2i AOptionButton::getPos() const
{
    vec2f pos = sprite_->getPosition();
    return vec2i( static_cast<int>( pos.x), static_cast<int>( pos.y));
}


vec2u AOptionButton::getSize() const
{
    return sprite_->getSize();
}


void AOptionButton::setParent(const IWindow* parent)
{
    parent_ = dynamic_cast<const IBar *>( parent);
    assert( parent_ );
}


void AOptionButton::setPos(const vec2i &pos)
{
    sprite_->setPosition( pos.x, pos.y);
}


void AOptionButton::setSize(const vec2u &size)
{
    assert( 0 && "Not implemented" );
}


void AOptionButton::forceActivate()
{
    assert( 0 && "Not implemented" );
}


void AOptionButton::forceDeactivate()
{
    assert( 0 && "Not implemented" );
}


bool AOptionButton::isActive() const
{
    assert( 0 && "Not implemented" );

    return false;
}


bool AOptionButton::isWindowContainer() const
{
    return false;
}


void AOptionButton::setState(State state)
{
    state_ = state;
}


IBarButton::State AOptionButton::getState() const
{
    return state_;
}


Bar::Bar( wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape, std::unique_ptr<sfm::RectangleShape> &normal,
                                                    std::unique_ptr<sfm::RectangleShape> &onHover,
                                                    std::unique_ptr<sfm::RectangleShape> &pressed,
                                                    std::unique_ptr<sfm::RectangleShape> &released)
    :   id_( init_id), size_( main_shape->getSize()), pos_( vec2i( main_shape->getPosition().x, main_shape->getPosition().y))
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

    for ( const auto &button : buttons_ )
    {
        button->draw( renderWindow);
        finishButtonDraw( renderWindow, button.get());

    }
}


std::unique_ptr<IAction> Bar::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


void Bar::addWindow(std::unique_ptr<IWindow> window)
{
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


bool Bar::unPressAllButtons()
{
    for ( auto &button : buttons_ )
    {
        button->setState( IBarButton::State::Normal);
    }
    return true;
}


void Bar::setPos(const vec2i &pos)
{
    pos_ = pos;
    main_shape_->setPosition( pos);
}


void Bar::setSize(const vec2u &size)
{
    size_ = size;
    main_shape_->setSize( size);
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


sfm::Color ColorPalette::getColor() const
{
    return color_;
}


float ThicknessBar::getThickness() const
{
    return thickness_;
}


float OpacityBar::getOpacity() const
{
    return opacity_;
}


InstrumentsBar::InstrumentsBar(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape)
    :   id_( init_id), main_shape_(std::move(main_shape)),
        size_( main_shape->getSize()),
        pos_( vec2i( main_shape->getPosition().x, main_shape->getPosition().y))
{}


void InstrumentsBar::draw(IRenderWindow* renderWindow)
{
    assert( renderWindow );

    renderWindow->draw( main_shape_.get());

    for ( const auto &instrument : instruments_ )
    {
        instrument->draw( renderWindow);
    }
}


std::unique_ptr<IAction> InstrumentsBar::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


wid_t InstrumentsBar::getId() const
{
    return id_;
}


const IWindow* InstrumentsBar::getWindowById(wid_t id) const
{
    for ( const auto &instrument : instruments_ )
    {
        if ( instrument->getId() == id )
        {
            return instrument.get();
        }
    }
    return nullptr;
}


IWindow* InstrumentsBar::getWindowById(wid_t id)
{
    for ( auto &instrument : instruments_ )
    {
        if ( instrument->getId() == id )
        {
            return instrument.get();
        }
    }
    return nullptr;
}


vec2i InstrumentsBar::getPos() const
{
    return pos_;
}


vec2u InstrumentsBar::getSize() const
{
    return size_;
}


void InstrumentsBar::setParent(const IWindow* parent)
{
    parent_ = parent;
}


void InstrumentsBar::setPos(const vec2i &pos)
{
    pos_ = pos;

    main_shape_->setPosition( pos);
}


void InstrumentsBar::setSize(const vec2u &size)
{
    size_ = size;

    main_shape_->setSize( size);
}


void InstrumentsBar::forceActivate()
{
    is_active_ = true;
}


void InstrumentsBar::forceDeactivate()
{
    is_active_ = false;
}


bool InstrumentsBar::isActive() const
{
    return is_active_;
}


bool InstrumentsBar::isWindowContainer() const
{
    return true;
}


void InstrumentsBar::addWindow(std::unique_ptr<IWindow> window)
{
    IBarButton *button = dynamic_cast<IBarButton *>( window.release());
    assert( button );

    instruments_.push_back( std::unique_ptr<IBarButton>( button));
}


void InstrumentsBar::removeWindow(wid_t id)
{
    for ( auto it = instruments_.begin(); it != instruments_.end(); ++it )
    {
        if ( (*it)->getId() == id )
        {
            instruments_.erase( it);
            return;
        }
    }
}


void InstrumentsBar::removeAllInstruments()
{
    instruments_.clear();
}


