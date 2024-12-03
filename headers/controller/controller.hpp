#ifndef PROGRAM_CONTROLLER
#define PROGRAM_CONTROLLER


#include <gui/window_manager.hpp>
#include <api_impl/windows.hpp>
#include <api_impl/actions.hpp>


class Controller
{
    RootWindow *root_window_;

    sfm::Event last_event_;
public:
    Controller(IWindowContainer *init_root);

    bool inProgress();
    void getRequests();
    void proceedView();
};


#endif // PROGRAM_CONTROLLER
