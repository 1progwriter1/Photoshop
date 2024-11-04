#include "api_photoshop.hpp"
#include "api_bar.hpp"

extern "C"
{


bool loadPlugin();
void unloadPlugin();


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool loadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    psapi::IWindow *toolbar = kRootWindowPtr->getWindowById( psapi::kToolBarWindowId);



    return true;
}




void unloadPlugin()
{

}


} // "C"
