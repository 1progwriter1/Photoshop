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

    std::unique_ptr<sfm::RectangleShape> main = std::make_unique<sfm::RectangleShape>();
    main->setPosition( sfm::vec2i( 10, 10));
    main->setSize( sfm::vec2u( 100, 880));
    main->setFillColor( sfm::Color( 204, 255, 255));
    main->setOutlineColor( sfm::Color( 51, 153, 255));
    main->setOutlineThickness( 10);

    std::unique_ptr<sfm::RectangleShape> normal = std::make_unique<sfm::RectangleShape>();
    normal->setSize( vec2u( 48, 48));
    normal->setFillColor( sfm::Color());
    normal->setOutlineColor( sfm::Color());
    normal->setOutlineThickness( 5);

    std::unique_ptr<sfm::RectangleShape> onHover = std::make_unique<sfm::RectangleShape>();
    onHover->setSize( vec2u( 48, 48));
    onHover->setFillColor( sfm::Color());
    onHover->setOutlineColor( sfm::Color( 102, 178, 255));
    onHover->setOutlineThickness( 5);

    std::unique_ptr<sfm::RectangleShape> pressed = std::make_unique<sfm::RectangleShape>();
    pressed->setSize( vec2u( 48, 48));
    pressed->setFillColor( sfm::Color());
    pressed->setOutlineColor( sfm::Color( 51, 153, 255));
    pressed->setOutlineThickness( 5);

    std::unique_ptr<sfm::RectangleShape> released = std::make_unique<sfm::RectangleShape>();
    released->setSize( vec2u( 48, 48));
    released->setFillColor( sfm::Color());
    released->setOutlineColor( sfm::Color( 153, 204, 255));
    released->setOutlineThickness( 5);

    std::unique_ptr<psapi::IBar> bar = std::make_unique<Bar>( main,
                                                                normal,
                                                                onHover,
                                                                pressed,
                                                                released);

    kRootWindowPtr->addWindow( std::move( bar));

    return true;
}


void unloadPlugin()
{

}


}
