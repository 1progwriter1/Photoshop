#include <controller/controller.hpp>
#include <api_actions.hpp>

#include <cassert>


Controller::Controller(IWindowContainer *init_root_window)
    :   root_window_( dynamic_cast<RootWindow *>( init_root_window)), last_event_( sfm::Event::None)
{
    assert( init_root_window );
    assert( root_window_ && "Failed to case WindowContainer to RootWindow" );
}


bool Controller::inProgress()
{
    return root_window_->getRenderWindow()->isOpen();
}


void Controller::getRequests()
{
    Event event;
    while ( root_window_->getRenderWindow()->pollEvent( event) )
    {
        if ( event.type == Event::Closed )
        {
            root_window_->getRenderWindow()->close();
        }
        last_event_ = event;
        psapi::getActionController()->execute(root_window_->createAction(root_window_->getRenderWindow(), event));
    }
    event.type = Event::EventType::None;
    psapi::getActionController()->execute(root_window_->createAction(root_window_->getRenderWindow(), event));
}


void Controller::proceedView()
{
    root_window_->getRenderWindow()->clear();
    root_window_->draw(root_window_->getRenderWindow());
    root_window_->getRenderWindow()->display();
}
