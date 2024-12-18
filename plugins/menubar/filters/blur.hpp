#ifndef BLUR_FILTER_PLUGIN
#define BLUR_FILTER_PLUGIN


#include "../menu_items.hpp"
#include "api_impl/canvas.hpp"


class BlurFilter : public TextButton
{
    friend class BlurFilterAction;
public:
    BlurFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class BlurFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    BlurFilter *button_;
public:
    BlurFilterAction(BlurFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};



#endif // BARELIEF_FILTER_PLUGIN
