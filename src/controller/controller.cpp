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
    sf::Event event;
    while ( view_->screen_.pollEvent( event) )
    {
        if ( event.type == sf::Event::Closed )
        {
            view_->screen_.close();
        }
    }
}


void Controller::proceedView()
{

}


void Controller::proceedModel()
{

}
