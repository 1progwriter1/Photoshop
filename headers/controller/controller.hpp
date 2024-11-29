#ifndef PROGRAM_CONTROLLER
#define PROGRAM_CONTROLLER


#include <model/model.hpp>
#include <gui/window_manager.hpp>
#include <api_impl/windows.hpp>
#include <api_impl/actions.hpp>


class Controller
{
    Model *model_;
    RootWindow *root_window_;

    sfm::Event last_event_;

    ActionsController actions_controller_;
public:
    Controller( Model *init_model, IWindowContainer *init_root);
    ~Controller();

    bool inProgress();
    void getRequests();
    void proceedModel();
    void proceedView();
};


#endif // PROGRAM_CONTROLLER
