#ifndef PLUGIN_MENU_FILE
#define PLUGIN_MENU_FILE


#include "api/api_canvas.hpp"
#include "menu_items.hpp"


template<typename type>
std::unique_ptr<TextButton> createFileButton(wid_t init_id, std::string &name, std::string &file)
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

    std::unique_ptr<type> filter = std::make_unique<type>(init_id, std::move(font),
                                                                std::move(button_text),
                                                                std::move(shape), file);
    return filter;
}


class OpenFile : public TextButton
{
    std::string file_;
    friend class OpenFileAction;
public:
    OpenFile(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape, std::string &file);

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class OpenFileAction : public AAction
{
    OpenFile *button_;
    ICanvas *canvas_;
public:
    OpenFileAction(OpenFile *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


class SaveFile : public TextButton
{
    std::string file_;
    friend class SaveFileAction;
public:
    SaveFile(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape, std::string &file);

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class SaveFileAction : public AAction
{
    SaveFile *button_;
    ICanvas *canvas_;
public:
    SaveFileAction(SaveFile *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


#endif // PLUGIN_MENU_FILE
