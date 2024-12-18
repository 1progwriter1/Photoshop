#ifndef NEGATIVE_FILTER_PLUGIN
#define NEGATIVE_FILTER_PLUGIN


#include "../menu_items.hpp"
#include "api_impl/canvas.hpp"


class NegativeFilter : public TextButton
{
    friend class NegativeFilterAction;
public:
    NegativeFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class NegativeFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    NegativeFilter *button_;
public:
    NegativeFilterAction(NegativeFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


#endif // NEGATIVE_FILTER_PLUGIN
