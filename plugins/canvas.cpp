#include "api_canvas.hpp"
#include "api_photoshop.hpp"
#include "../headers/api_impl/canvas.hpp"
#include <memory>

extern "C"
{


bool loadPlugin();
void unloadPlugin();


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool loadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    std::unique_ptr<psapi::ICanvas> canvas = std::make_unique<Canvas>( psapi::vec2i( 120, 0), psapi::vec2u( 1480, 900 * 3));

    kRootWindowPtr->addWindow( std::move( canvas));

    return true;
}




void unloadPlugin()
{

}


} // "C"
