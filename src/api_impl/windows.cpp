#include <api_impl/windows.hpp>
#include <cassert>


using namespace psapi;


IWindow::~IWindow()
{

}

bool AWindow::isActive() const
{
    return is_active_;
}


void AWindow::forceActivate()
{
    is_active_ = true;
}


void AWindow::forceDeactivate()
{
    is_active_ = false;
}


vec2i AWindow::getPos() const
{
    return pos_;
}


vec2u AWindow::getSize() const
{
    return size_;
}


wid_t AWindow::getId() const
{
    return id_;
}


void AWindow::setParent( const IWindow *new_parent)
{
    parent_ = new_parent;
}


bool AWindow::isWindowContainer() const
{
    return false;
}


IWindow *AWindow::getWindowById( wid_t id)
{
    return id == id_ ? this : nullptr;
}


const IWindow *AWindow::getWindowById( wid_t id) const
{
    return id == id_ ? this : nullptr;
}


void WindowContainer::draw( sfm::IRenderWindow *renderWindow)
{
    for ( auto &window : windows_ )
    {
        window->draw( renderWindow);
    }
}


bool WindowContainer::update( const IRenderWindow* renderWindow, const Event& event)
{
    assert( 0 && "Not implemented" );

    return true;
}


IWindow *WindowContainer::getWindowById( wid_t id)
{
    for ( auto &window : windows_ )
    {
        if ( window->getId() == id )
        {
            return window->getWindowById( id);
        }
    }

    return nullptr;
}


const IWindow *WindowContainer::getWindowById( wid_t id) const
{
    for ( auto &window : windows_ )
    {
        if ( window->getId() == id )
        {
            return window->getWindowById( id);
        }
    }

    return nullptr;
}


void WindowContainer::addWindow( std::unique_ptr<IWindow> window)
{
    windows_.insert( windows_.end(), std::move( window));
}


void WindowContainer::removeWindow( wid_t id)
{
    for ( auto &window : windows_ )
    {
        if ( window->getId() == id )
        {
            windows_.remove( window);
        }
    }
}


bool WindowContainer::isWindowContainer() const
{
    return true;
}


void WindowVector::draw( IRenderWindow *renderWindow)
{
    for ( auto &window : windows_ )
    {
        window->draw( renderWindow);
    }
}


bool WindowVector::update( const IRenderWindow *renderWindow, const Event &event)
{
    assert( 0 && "Not implemented" );

    return true;
}


void WindowVector::addWindow( std::unique_ptr<IWindow> window)
{
    windows_.push_back( std::move( window));
}


void WindowVector::removeWindow( wid_t id)
{
    size_t w_cnt = windows_.size();
    for ( size_t i = 0; i < w_cnt; i++ )
    {
        if ( windows_[i]->getId() == id )
            windows_.erase( windows_.begin() + i);
    }
}


IWindow *WindowVector::getWindowById( wid_t id)
{
    for ( auto &window : windows_ )
    {
        if ( window->getId() == id )
        {
            window->getWindowById( id);
        }
    }
}


const IWindow *WindowVector::getWindowById( wid_t id) const
{
    for ( auto &window : windows_ )
    {
        if ( window->getId() == id )
        {
            window->getWindowById( id);
        }
    }
}


generalFunction getGeneralFunction(const std::string &name)
{
    assert( 0 && "generalFunction is not implemented" );
}
