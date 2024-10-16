#include <controller/controller.hpp>

#include <cassert>


Controller::Controller( Model *init_model, WindowManager *init_view)
    :   model_( init_model), view_( init_view)
{
    assert( init_model );
    assert( init_view );
}


bool Controller::inProgress()
{
    return view_->screen_.isOpen();
}


void Controller::getRequests()
{
    while
}


void Controller::proceedView()
{

}


void Controller::proceedModel()
{

}
