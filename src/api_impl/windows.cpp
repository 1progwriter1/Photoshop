#include <api_impl/windows.hpp>
#include <cassert>
#include <iostream>
#include <sys/my_exceptions.hpp>


using namespace psapi;


sfm::vec2i psapi::getScreenSize()
{
    return psapi::getRootWindow()->getSize();
}


sfm::IntRect psapi::getToolbarIntRect()
{
    return sfm::IntRect( sfm::vec2i( 0, 0), sfm::vec2u( 84, psapi::getScreenSize().y));
}


sfm::IntRect psapi::getCanvasIntRect()
{
    return sfm::IntRect( sfm::vec2i( 84, 84), sfm::vec2u( psapi::getScreenSize().x - 84 * 2, psapi::getScreenSize().y - 84));
}


sfm::IntRect psapi::getOptionsBarIntRect()
{
    return sfm::IntRect( sfm::vec2i( 84, 0), sfm::vec2u(psapi::getScreenSize().x - 84 * 2, 84));
}


sfm::IntRect psapi::getInstrumentOptionsIntRect()
{
    return sfm::IntRect( sfm::vec2i( psapi::getScreenSize().x - 84, 0), sfm::vec2u( 84, psapi::getScreenSize().y));
}


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


IRootWindow *RootWindow::getInstance()
{
    static RootWindow window;
    return &window;
}


IRootWindow *psapi::getRootWindow()
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


void RootWindow::draw(sfm::IRenderWindow *renderWindow)
{
    assert( renderWindow );

    for ( auto &window : windows_ )
    {
        window->draw( renderWindow);
    }
}


std::unique_ptr<IAction> RootWindow::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<RootWindowAction>(&windows_, renderWindow, event);
}


layer_id_t RootWindow::getUpperLayerId() const
{
    return active_layer_id_;
}


layer_id_t RootWindow::increaseLayerId()
{
    return ++active_layer_id_;
}


layer_id_t RootWindow::decreaseLayerId()
{
    return --active_layer_id_;
}


bool RootWindow::isWindowContainer() const
{
    return true;
}


void RootWindow::addWindow(std::unique_ptr<IWindow> window)
{
    windows_.insert( windows_.end(), std::move( window));
}


void RootWindow::removeWindow(wid_t id)
{
    auto iter = windows_.begin();
    for ( auto &window : windows_ )
    {
        if ( window->getId() == id )
        {
            windows_.erase( iter);
            break;
        }
        iter++;
    }
}


RootWindowAction::RootWindowAction(std::vector<std::unique_ptr<IWindow>> *windows, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), windows_( windows) {}


bool RootWindowAction::isUndoable(const Key &key)
{
    return false;
}


bool RootWindowAction::execute(const Key &key)
{
    for ( auto &window : *windows_ )
    {
        psapi::getActionController()->execute(window->createAction(render_window_, event_));
    }
    return true;
}
