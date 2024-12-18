#ifndef BARELIEF_FILTER_PLUGIN
#define BARELIEF_FILTER_PLUGIN


#include "../menu_items.hpp"
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


#endif // BARELIEF_FILTER_PLUGIN
