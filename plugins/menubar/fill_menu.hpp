#ifndef PLUGIN_FILL_MENU
#define PLUGIN_FILL_MENU


#include "menubar.hpp"
#include "api/api_canvas.hpp"
#include "menu_items.hpp"
#include <cassert>


extern "C"
{

bool loadPlugin();
void unloadPlugin();

}



template<typename filter_type>
std::unique_ptr<IWindow> createFilter(wid_t id, const std::string &name)
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

    std::unique_ptr<filter_type> filter = std::make_unique<filter_type>(id, std::move(font),
                                                                            std::move(button_text),
                                                                            std::move(shape));
    return filter;
}



#endif
