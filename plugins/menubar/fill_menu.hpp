#ifndef PLUGIN_FILL_MENU
#define PLUGIN_FILL_MENU


#include "menu_items.hpp"
#include "file.hpp"
#include "tools.hpp"
#include "layer.hpp"


using namespace psapi;


extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

void fillMenu(ABar *menu);
void fillFileMenu(AMenuButton* menu_bar);
void fillFilterMenu(AMenuButton* menu_bar);
void fillLayerMenu(AMenuButton* menu_bar);
void fillToolsMenu(AMenuButton* menu_bar);
void fillHelpMenu(AMenuButton* menu_bar);


template<typename type>
std::unique_ptr<IWindow> createMenuButton(wid_t id, const std::string &name)
{
    std::unique_ptr<sfm::IFont> font = sfm::IFont::create();
    font->loadFromFile(FONT_FILE);
    std::unique_ptr<sfm::IText> button_text = sfm::IText::create();
    button_text->setFont(font.get());
    sfm::Color color = sfm::Color::getStandardColor(psapi::sfm::Color::Type::Black);
    button_text->setFillColor(&color);
    button_text->setString(name);
    sfm::IntRect text_rect = button_text->getGlobalBounds();
    std::unique_ptr<sfm::IRectangleShape> shape = sfm::RectangleShape::create(10, 10); // default size, it is not used later
    shape->setFillColor(NESTED_BAR_COLOR);

    std::unique_ptr<type> filter = std::make_unique<type>(id, std::move(font),
                                                                std::move(button_text),
                                                                std::move(shape));
    return filter;
}


std::pair<std::unique_ptr<IWindow>, std::unique_ptr<IWindow>> createZoomButtons();


#endif
