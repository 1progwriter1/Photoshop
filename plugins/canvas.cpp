#include "api/api_canvas.hpp"
#include "api/api_photoshop.hpp"
#include "../headers/api_impl/canvas.hpp"
#include <memory>

extern "C"
{


bool onLoadPlugin();
void onUnloadPlugin();


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool onLoadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    sfm::IntRect rect = psapi::getCanvasIntRect();
    rect.size.x *= 2;
    rect.size.y *= 3;
    std::unique_ptr<psapi::ICanvas> canvas = std::make_unique<Canvas>(rect.pos, rect.size);

    kRootWindowPtr->addWindow( std::move( canvas));

    return true;
}




void onUnloadPlugin()
{

}


} // "C"
