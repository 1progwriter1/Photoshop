#ifndef PROGRAM_CONTROLLER
#define PROGRAM_CONTROLLER


#include <model/model.hpp>
#include <gui/window_manager.hpp>


class Controller
{
    Model *model_;
    WindowManager *view_;

public:
    Controller( Model *init_model, WindowManager *init_view);
    ~Controller() = default;

    bool inProgress();
    void getRequests();
    void proceedModel();
    void proceedView();
};


#endif // PROGRAM_CONTROLLER
