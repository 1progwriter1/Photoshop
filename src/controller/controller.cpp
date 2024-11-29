#include <controller/controller.hpp>

#include <cassert>
#include <iostream>


ActionsController *actions_controller = nullptr;


Controller::Controller( Model *init_model, IWindowContainer *init_root_window)
    :   model_( init_model), root_window_( dynamic_cast<RootWindow *>( init_root_window)), last_event_( sfm::Event::None)
{
    assert( init_model );
    assert( init_root_window );
    assert( root_window_ && "Failed to case WindowContainer to RootWindow" );

    if ( !actions_controller )
        actions_controller = new ActionsController();

    actions_controller_ = *actions_controller;
}


Controller::~Controller()
{
    delete actions_controller;
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
        actions_controller_.execute(root_window_->createAction(root_window_->getRenderWindow(), event));
    }
}


void Controller::proceedView()
{
    root_window_->getRenderWindow()->clear();
    root_window_->draw( root_window_->getRenderWindow());
    root_window_->getRenderWindow()->display();
}


void Controller::proceedModel()
{

}


AActionController *getActionController()
{
    return actions_controller;
}
