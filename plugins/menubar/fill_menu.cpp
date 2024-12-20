#include "fill_menu.hpp"
#include <cassert>
#include "api/api_photoshop.hpp"
#include "windows_id.hpp"
#include <filesystem>
#include <iostream>
#include "filters/unsharp_mask.hpp"
#include "filters/gauss_blur.hpp"
#include "filters/blur.hpp"
#include "filters/barelief.hpp"
#include "filters/brightness.hpp"
#include "filters/negative.hpp"



IRootWindow *kRootWindowPtr = nullptr;


void addFiles(AMenuButton *open_bar, AMenuButton *close_bar);


bool onLoadPlugin()
{
    kRootWindowPtr = psapi::getRootWindow();

    ABar *menu = dynamic_cast<ABar *>(kRootWindowPtr->getWindowById(kMenuBarWindowId));
    assert( menu && "Failed to cast to menu" );
    fillMenu(menu);

    AMenuButton *filter_bar = dynamic_cast<AMenuButton *>(menu->getWindowById(kMenuFilterId));
    assert( filter_bar && "Failed to cast to menu button" );
    fillFilterMenu(filter_bar);

    AMenuButton *file_bar = dynamic_cast<AMenuButton *>(menu->getWindowById(kMenuFileId));
    assert( file_bar && "Failed to cast to menu button" );
    fillFileMenu(file_bar);

    AMenuButton *layer_bar = dynamic_cast<AMenuButton *>(menu->getWindowById(kMenuLayerId));
    assert( layer_bar && "Failed to cast to menu button" );
    fillLayerMenu(layer_bar);


    AMenuButton *tools_bar = dynamic_cast<AMenuButton *>(menu->getWindowById(kMenuToolsId));
    assert( tools_bar && "Failed to cast to menu button" );
    fillToolsMenu(tools_bar);

    AMenuButton *help_bar = dynamic_cast<AMenuButton *>(menu->getWindowById(kMenuHelpId));
    assert( help_bar && "Failed to cast to menu button" );
    fillHelpMenu(help_bar);

    return true;
}


void onUnloadPlugin()
{

}


void fillMenu(ABar *menu)
{
    assert( menu );

    menu->addWindow(MenuButton::createMenuButton(kMenuFileId, "File"));
    menu->addWindow(MenuButton::createMenuButton(kMenuFilterId, "Filter"));
    menu->addWindow(MenuButton::createMenuButton(kMenuLayerId, "Layer"));
    menu->addWindow(MenuButton::createMenuButton(kMenuToolsId, "Tools"));
    menu->addWindow(MenuButton::createMenuButton(kMenuHelpId, "Help"));
}


void fillFileMenu(AMenuButton* menu_bar)
{
    assert( menu_bar );

    menu_bar->addMenuItem(NestedMenuButton::createNestedMenuButton(kOpenFileMenuId, "Open", menu_bar->getMenu()));
    menu_bar->addMenuItem(NestedMenuButton::createNestedMenuButton(kSaveFileMenuId, "Save", menu_bar->getMenu()));

    AMenuButton *open_bar = dynamic_cast<AMenuButton *>(menu_bar->getMenu()->getWindowById(kOpenFileMenuId));
    assert( open_bar && "Failed to cast to menu button" );

    AMenuButton *save_bar = dynamic_cast<AMenuButton *>(menu_bar->getMenu()->getWindowById(kSaveFileMenuId));
    assert( save_bar && "Failed to cast to menu button" );

    addFiles(open_bar, save_bar);
}


void fillFilterMenu(AMenuButton* menu_bar)
{
    assert( menu_bar );

    menu_bar->addMenuItem(createMenuButton<BareliefFilter>(kBareliefFilterButtonId, "Barelief"));
    menu_bar->addMenuItem(createMenuButton<UnsharpMaskFilter>(kContrastFilterButtonId, "UnsharpMask"));
    menu_bar->addMenuItem(createMenuButton<BlurFilter>(kBlurFilterButtonId, "Blur"));
    menu_bar->addMenuItem(createMenuButton<NegativeFilter>(kNegativeFilterButtonId, "Negative"));
    menu_bar->addMenuItem(createMenuButton<BrightnessFilter>(kBrightnessFilterButtonId, "Brightness"));
}


void fillLayerMenu(AMenuButton* menu_bar)
{
    assert( menu_bar );

    std::pair<std::unique_ptr<IWindow>, std::unique_ptr<IWindow>> zoom = createZoomButtons();

    menu_bar->addMenuItem(std::move(zoom.first));
    menu_bar->addMenuItem(std::move(zoom.second));
    menu_bar->addMenuItem(createMenuButton<Undo>(kUndoButtonId, "Undo"));
    menu_bar->addMenuItem(createMenuButton<Redo>(kRedoButtonId, "Redo"));
}


void fillToolsMenu(AMenuButton* menu_bar)
{
    assert( menu_bar );

    menu_bar->addMenuItem(createMenuButton<Seagfault>(kSeagfaultButtonId, "Seagfault"));
}


void fillHelpMenu(AMenuButton* menu_bar)
{
    assert( menu_bar );
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


std::pair<std::unique_ptr<IWindow>, std::unique_ptr<IWindow>> createZoomButtons()
{
    std::unique_ptr<IWindow> zoom_button = createMenuButton<ZoomCanvas>(kZoomCanvasButtonId, "Zoom +");
    std::unique_ptr<IWindow> unzoom_button = createMenuButton<UnZoomCanvas>(kUnZoomCanvasButtonId, "Zoom -");

    ZoomCanvas *zoom_canvas = dynamic_cast<ZoomCanvas *>(zoom_button.get());
    UnZoomCanvas *unzoom_canvas = dynamic_cast<UnZoomCanvas *>(unzoom_button.get());

    zoom_canvas->another_ = unzoom_canvas;
    unzoom_canvas->another_ = zoom_canvas;

    return std::make_pair(std::move(zoom_button), std::move(unzoom_button));
}
