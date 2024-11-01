#include "../api/api_photoshop.hpp"
#include "../api/api_bar.hpp"
#include "../headers/api_impl/bar.hpp"


extern "C"
{


bool loadPlugin();
void unloadPlugin();


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool loadPlugin()
{
    kRootWindowPtr = psapi::getRootWindow();

    std::unique_ptr<psapi::IBar> bar = std::make_unique<ABar>()
}


void unloadPlugin()
{

}


}
