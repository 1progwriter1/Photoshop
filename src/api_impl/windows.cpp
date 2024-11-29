#include <api_impl/windows.hpp>
#include <cassert>
#include <iostream>
#include <sys/my_exceptions.hpp>


using namespace psapi;



bool IWindowContainer::isWindowContainer() const
{
    return true;
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


void AWindow::setPos( const vec2i &pos)
{
    pos_ = pos;
}


void AWindow::setSize( const vec2u &size)
{
    size_ = size;
}


void AWindowContainer::draw( sfm::IRenderWindow *renderWindow)
{
    for ( auto &window : windows_ )
    {
        window->draw( renderWindow);
    }
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


std::unique_ptr<IAction> AWindowContainer::createAction( const IRenderWindow *renderWindow, const Event &event)
{
    assert( 0 && "createAction is not implemented" );
    return nullptr;
}


bool AWindowContainer::isWindowContainer() const
{
    return true;
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


void RootWindow::setPos(const vec2i &pos)
{
    pos_ = pos;
}


void RootWindow::setSize(const vec2u &size)
{
    size_ = size;
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
