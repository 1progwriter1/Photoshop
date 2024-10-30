#include <controller/controller.hpp>

#include <cassert>


Controller::Controller( Model *init_model, IWindowContainer *init_root_window)
    :   model_( init_model), root_window_( dynamic_cast<RootWindow *>( init_root_window))
{
    assert( init_model );
    assert( init_root_window );
    assert( root_window_ && "Failed to case WindowContainer to RootWindow" );
}


bool Controller::inProgress()
{
    return root_window_->getRenderWindow().isOpen();
}


void Controller::getRequests()
{
    Event event;
    while ( root_window_->getRenderWindow().pollEvent( event) )
    {
        if ( event.type == Event::Closed )
        {
            root_window_->getRenderWindow().close();
        }
    }
}


void Controller::proceedView()
{
    root_window_->getRenderWindow().clear();
    root_window_->getRenderWindow().clear();
}


void Controller::proceedModel()
{

}
