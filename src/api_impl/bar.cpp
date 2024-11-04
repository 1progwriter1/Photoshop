#include <api_impl/bar.hpp>
#include <cassert>


void BarButton::draw(IRenderWindow* renderWindow)
{
    assert( 0 && "Not implemented" );
}


bool BarButton::update(const IRenderWindow* renderWindow, const Event& event)
{
    assert( 0 && "Not implemented" );

    return true;
}


wid_t BarButton::getId() const
{
    assert( 0 && "Not implemented" );

    return 0;
}


IWindow* BarButton::getWindowById(wid_t id)
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


const IWindow* BarButton::getWindowById(wid_t id) const
{
    assert( 0 && "Not implemented" );

    return nullptr;
}


vec2i BarButton::getPos() const
{
    assert( 0 && "Not implemented" );

    return vec2i();
}


vec2u BarButton::getSize() const
{
    assert( 0 && "Not implemented" );

    return vec2u();
}


void BarButton::setParent(const IWindow* parent)
{
    assert( 0 && "Not implemented" );
}


void BarButton::forceActivate()
{
    assert( 0 && "Not implemented" );
}


void BarButton::forceDeactivate()
{
    assert( 0 && "Not implemented" );
}


bool BarButton::isActive() const
{
    assert( 0 && "Not implemented" );

    return false;
}


bool BarButton::isWindowContainer() const
{
    assert( 0 && "Not implemented" );

    return false;
}


void BarButton::setState(State state)
{
    assert( 0 && "Not implemented" );
}


IBarButton::State BarButton::getState() const
{
    assert( 0 && "Not implemented" );

    return IBarButton::State::Normal;
}


Bar::Bar( std::unique_ptr<sfm::RectangleShape> main_shape, std::unique_ptr<sfm::RectangleShape> normal,
                                                    std::unique_ptr<sfm::RectangleShape> onHover,
                                                    std::unique_ptr<sfm::RectangleShape> pressed,
                                                    std::unique_ptr<sfm::RectangleShape> released)
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
    renderWindow->draw( main_shape_.get());
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
    switch ( button->getState() )
    {
        case IBarButton::State::Normal:
            renderWindow->draw( normal_.get());
            break;
        case IBarButton::State::Hover:
            renderWindow->draw( onHover_.get());
            break;
        case IBarButton::State::Press:
            renderWindow->draw( pressed_.get());
            break;
        case IBarButton::State::Released:
            renderWindow->draw( released_.get());
            break;
        default:
            assert( 0 && "Unreachable" );

    }
}
