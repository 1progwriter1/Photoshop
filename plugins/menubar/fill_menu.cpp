#include "fill_menu.hpp"
#include <cassert>



IRootWindow *kRootWindowPtr = nullptr;


bool loadPlugin()
{
    kRootWindowPtr = psapi::getRootWindow();

    ABar *menu = dynamic_cast<ABar *>(kRootWindowPtr->getWindowById(kMenuBarWindowId));
    assert( menu && "Failed to cast to menu" );

    menu->addWindow(MenuButton::createMenuButton(kMenuFileId, "File"));
    menu->addWindow(MenuButton::createMenuButton(kMenuFilterId, "Filter"));
    menu->addWindow(MenuButton::createMenuButton(kMenuLayerId, "Layer"));
    menu->addWindow(MenuButton::createMenuButton(kMenuToolsId, "Tools"));
    menu->addWindow(MenuButton::createMenuButton(kMenuHelpId, "Help"));

    return true;
}


void unloadPlugin()
{

}
