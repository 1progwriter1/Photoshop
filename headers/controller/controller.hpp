#ifndef PROGRAM_CONTROLLER
#define PROGRAM_CONTROLLER


#include <model/model.hpp>
#include <gui/window_manager.hpp>


class Controller
{
    Model *model_;
    RootWindow *root_window_;

public:
    Controller( Model *init_model, IWindowContainer *init_root);
    ~Controller() = default;

    bool inProgress();
    void getRequests();
    void proceedModel();
    void proceedView();
};


#endif // PROGRAM_CONTROLLER
