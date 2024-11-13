#include <api_impl/windows.hpp>
#include <cassert>
#include <iostream>
#include <sys/my_exceptions.hpp>


using namespace psapi;



bool IWindowContainer::isWindowContainer() const
{
    return true;
}


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
    assert( "here" );
    return id_;
}


void AWindow::setParent( const IWindow *new_parent)
{
    parent_ = new_parent;
}


IWindow *AWindow::getWindowById( wid_t id)
{
    return id == id_ ? this : nullptr;
}


const IWindow *AWindow::getWindowById( wid_t id) const
{
    return id == id_ ? this : nullptr;
}


void AWindowContainer::draw( sfm::IRenderWindow *renderWindow)
{
    for ( auto &window : windows_ )
    {
        window->draw( renderWindow);
    }
}


bool AWindowContainer::update( const IRenderWindow* renderWindow, const Event& event)
{
    assert( renderWindow );

    for ( auto &window : windows_ )
    {
        if ( !window->update( renderWindow, event) )
            return false;
    }

    return true;
}


IWindow *AWindowContainer::getWindowById( wid_t id)
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


const IWindow *AWindowContainer::getWindowById( wid_t id) const
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


void AWindowContainer::addWindow( std::unique_ptr<IWindow> window)
{
    windows_.insert( windows_.end(), std::move( window));
}


void AWindowContainer::removeWindow( wid_t id)
{
    for ( auto &window : windows_ )
    {
        if ( window->getId() == id )
        {
            windows_.remove( window);
        }
    }
}


bool AWindowContainer::isWindowContainer() const
{
    return true;
}


void AWindowVector::draw( IRenderWindow *renderWindow)
{
    for ( auto &window : windows_ )
    {
        window->draw( renderWindow);
    }
}


bool AWindowVector::update( const IRenderWindow *renderWindow, const Event &event)
{
    assert( 0 && "Not implemented" );

    return true;
}


void AWindowVector::addWindow( std::unique_ptr<IWindow> window)
{
    windows_.push_back( std::move( window));
}


void AWindowVector::removeWindow( wid_t id)
{
    size_t w_cnt = windows_.size();
    for ( size_t i = 0; i < w_cnt; i++ )
    {
        if ( windows_[i]->getId() == id )
            windows_.erase( windows_.begin() + i);
    }
}


IWindow *AWindowVector::getWindowById( wid_t id)
{
    for ( auto &window : windows_ )
    {
        if ( window->getId() == id )
        {
            window->getWindowById( id);
        }
    }
    return nullptr;
}


const IWindow *AWindowVector::getWindowById( wid_t id) const
{
    for ( auto &window : windows_ )
    {
        if ( window->getId() == id )
        {
            window->getWindowById( id);
        }
    }
    return nullptr;
}


generalFunction getGeneralFunction(const std::string &name)
{
    assert( 0 && "generalFunction is not implemented" );
}


RootWindow::RootWindow()
    :   window_( kRootWindowWidth, kRootWindowHeight, kRootWindowTitle)
{}


const IRenderWindow *RootWindow::getRenderWindow() const
{
    return &window_;
}


IRenderWindow *RootWindow::getRenderWindow()
{
    return &window_;
}


IWindowContainer *RootWindow::getInstance()
{
    static RootWindow window;
    return &window;
}


IWindowContainer *psapi::getRootWindow()
{
    return RootWindow::getInstance();
}


wid_t RootWindow::getId() const
{
    return kRootWindowId;
}


vec2u RootWindow::getSize() const
{
    return size_;
}


void RootWindow::setParent(const IWindow *parent)
{
    throw MY_EXCEPTION( "you cannot set parent for RootWindow", nullptr);
}


vec2i RootWindow::getPos() const
{
    return pos_;
}


void RootWindow::forceActivate()
{
    is_active_ = true;
}


void RootWindow::forceDeactivate()
{
    is_active_ = false;
}


bool RootWindow::isActive() const
{
    return is_active_;
}


void RootWindow::drawChildren( IRenderWindow *renderWindow)
{
    assert( 0 &&"Not implemented" );
}


IWindow *RootWindow::getWindowById( wid_t id)
{
    if ( id == kRootWindowId )  return this;

    for ( auto &window : windows_ )
    {
        if ( window->getId() == id )
        {
            return window->getWindowById( id);
        }
    }
    return nullptr;
}


const IWindow *RootWindow::getWindowById( wid_t id) const
{
    assert( 0 && "here" );
    if ( id == kRootWindowId )  return this;

    for ( auto &window : windows_ )
    {
        if ( window->getId() == id )
        {
            return window->getWindowById( id);
        }
    }
    return nullptr;
}
