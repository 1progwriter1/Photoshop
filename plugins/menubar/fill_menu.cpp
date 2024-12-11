#include "fill_menu.hpp"
#include <cassert>
#include "api/api_photoshop.hpp"
#include "windows_id.hpp"
#include <filesystem>
#include <iostream>

#include "filters.hpp"
#include "file.hpp"


IRootWindow *kRootWindowPtr = nullptr;


void addFiles(AMenuButton *open_bar, AMenuButton *close_bar);


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

    filter_bar->addMenuItem(createMenuButton<BareliefFilter>(kBareliefFilterButtonId, "Barelief"));
    filter_bar->addMenuItem(createMenuButton<ContrastFilter>(kContrastFilterButtonId, "Contrast"));
    filter_bar->addMenuItem(createMenuButton<BlurFilter>(kBlurFilterButtonId, "Blur"));
    filter_bar->addMenuItem(createMenuButton<GaussBlurFilter>(kGaussBlurFilterButtonId, "Gauss blur"));
    filter_bar->addMenuItem(createMenuButton<NegativeFilter>(kNegativeFilterButtonId, "Negative"));
    filter_bar->addMenuItem(createMenuButton<BrightnessFilter>(kBrightnessFilterButtonId, "Brightness"));

    AMenuButton *file_bar = dynamic_cast<AMenuButton *>(menu->getWindowById(kMenuFileId));
    assert( file_bar && "Failed to cast to menu button" );

    file_bar->addMenuItem(NestedMenuButton::createNestedMenuButton(kOpenFileMenuId, "Open", file_bar->getMenu()));
    file_bar->addMenuItem(NestedMenuButton::createNestedMenuButton(kSaveFileMenuId, "Save", file_bar->getMenu()));

    AMenuButton *open_bar = dynamic_cast<AMenuButton *>(file_bar->getMenu()->getWindowById(kOpenFileMenuId));
    assert( open_bar && "Failed to cast to menu button" );

    AMenuButton *save_bar = dynamic_cast<AMenuButton *>(file_bar->getMenu()->getWindowById(kSaveFileMenuId));
    assert( save_bar && "Failed to cast to menu button" );

    addFiles(open_bar, save_bar);

    return true;
}


void unloadPlugin()
{

}


void addFiles(AMenuButton *open_bar, AMenuButton *save_bar)
{
    assert( open_bar );
    assert( save_bar );

    std::string directory = "../images";
    if ( std::filesystem::exists(directory) && std::filesystem::is_directory(directory ))
    {
        for (const auto& entry : std::filesystem::directory_iterator(directory))
        {
            if ( std::filesystem::is_regular_file(entry))
            {
                std::string filename = entry.path().filename().string();
                std::string path = entry.path().string();
                open_bar->addMenuItem(createFileButton<OpenFile>(-1, filename, path));
                filename = entry.path().filename().string();
                path = entry.path().string();
                save_bar->addMenuItem(createFileButton<SaveFile>(-1, filename, path));
            }
        }
    } else
    {
        std::cerr << "Указанный путь не существует или не является директорией." << std::endl;
    }
}
