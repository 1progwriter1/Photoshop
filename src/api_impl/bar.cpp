#include <api_impl/bar.hpp>
#include <cassert>


void ABarButton::draw(IRenderWindow* renderWindow)
{
    assert( 0 && "Not implemented" );
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
    assert( 0 && "Not implemented" );

    return vec2i();
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
    assert( 0 && "Not implemented" );

    return IBarButton::State::Normal;
}


Bar::Bar( vec2u init_size, vec2i init_pos, const sfm::Color &in_color, const sfm::Color &out_color, const float thickness)
    :   size_( init_size), pos_( init_pos)
{
    shape_.setSize( init_size);
    shape_.setFillColor( in_color);
    shape_.setPosition( init_pos);
    shape_.setOutlineColor( out_color);
    shape_.setOutlineThickness( thickness);
}


void Bar::draw(IRenderWindow* renderWindow)
{
    shape_.draw( renderWindow);
}


bool Bar::update(const IRenderWindow* renderWindow, const Event& event)
{
    assert( 0 && "Not implemented" );

    return true;
}


void Bar::addWindow(std::unique_ptr<IWindow> window)
{
    assert( 0 && "Not implemented" );
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
    assert( 0 && "Not implemented" );
}
