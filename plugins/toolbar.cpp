#include "../api/api_photoshop.hpp"
#include "../api/api_bar.hpp"
#include "../headers/api_impl/bar.hpp"
#include <memory>


extern "C"
{


bool loadPlugin();
void unloadPlugin();


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool loadPlugin()
{
    kRootWindowPtr = psapi::getRootWindow();

    std::unique_ptr<psapi::IBar> bar = std::make_unique<Bar>( sfm::vec2u( 100, 880), sfm::vec2i( 10, 10),
                                                                sfm::Color( 204, 153, 255), sfm::Color( 102, 0, 204),
                                                                10);

    kRootWindowPtr->addWindow( std::move( bar));

    return true;
}


void unloadPlugin()
{

}


}
