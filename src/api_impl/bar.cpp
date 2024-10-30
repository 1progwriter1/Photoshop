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


void ABar::draw(IRenderWindow* renderWindow)
{
    assert( 0 && "Not implemented" );
}


bool ABar::update(const IRenderWindow* renderWindow, const Event& event)
{
    assert( 0 && "Not implemented" );

    return true;
}


wid_t ABar::getId() const
{
    assert( 0 && "Not implemented" );

    return 0;
}


IWindow* ABar::getWindowById(wid_t id)
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


const IWindow* ABar::getWindowById(wid_t id) const
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


vec2i ABar::getPos() const
{
    assert( 0 && "Not implemented" );

    return vec2i();
}


vec2u ABar::getSize() const
{
    assert( 0 && "Not implemented" );

    return vec2u();
}


void ABar::setParent(const IWindow* parent)
{
    assert( 0 && "Not implemented" );
}


void ABar::forceActivate()
{
    assert( 0 && "Not implemented" );
}


void ABar::forceDeactivate()
{
    assert( 0 && "Not implemented" );
}


bool ABar::isActive() const
{
    assert( 0 && "Not implemented" );

    return false;
}


bool ABar::isWindowContainer() const
{
    assert( 0 && "Not implemented" );

    return false;
}


ChildInfo ABar::getNextChildInfo() const
{
    assert( 0 && "Not implemented" );

    return ChildInfo();
}


void ABar::finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const
{
    assert( 0 && "Not implemented" );
}
