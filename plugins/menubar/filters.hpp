#ifndef FILTERS_FOR_MENU
#define FILTERS_FOR_MENU


#include "menu_items.hpp"
#include "api/api_canvas.hpp"


class BareliefFilter : public TextButton
{
    friend class BareliefFilterAction;
public:
    BareliefFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class BareliefFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    BareliefFilter *button_;
public:
    BareliefFilterAction(BareliefFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


class ContrastFilter : public TextButton
{
    friend class ContrastFilterAction;
public:
    ContrastFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


sfm::Color createContrastColor( const sfm::Color &normal, const sfm::Color &blurred, int cf);


class ContrastFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    ContrastFilter *button_;
public:
    ContrastFilterAction(ContrastFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


#endif // FILTERS_FOR_MENU
