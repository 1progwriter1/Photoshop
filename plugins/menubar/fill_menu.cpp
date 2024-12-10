#include "fill_menu.hpp"
#include <cassert>
#include "api/api_photoshop.hpp"
#include "filters.hpp"
#include "windows_id.hpp"


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

    AMenuButton *filter_bar = dynamic_cast<AMenuButton *>(menu->getWindowById(kMenuFilterId));
    assert( filter_bar && "Failed to cast to menu button" );

    filter_bar->addMenuItem(createFilter<BareliefFilter>(kBareliefFilterButtonId, "Barelief"));
    filter_bar->addMenuItem(createFilter<ContrastFilter>(kContrastFilterButtonId, "Contrast"));
    filter_bar->addMenuItem(createFilter<BlurFilter>(kBlurFilterButtonId, "Blur"));
    filter_bar->addMenuItem(createFilter<GaussBlurFilter>(kGaussBlurFilterButtonId, "Gauss blur"));
    filter_bar->addMenuItem(createFilter<NegativeFilter>(kNegativeFilterButtonId, "Negative"));
    filter_bar->addMenuItem(createFilter<BrightnessFilter>(kBrightnessFilterButtonId, "Brightness"));

    return true;
}


void unloadPlugin()
{

}
