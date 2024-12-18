#ifndef GAUSS_BLUR_FILTER_PLUGIN
#define GAUSS_BLUR_FILTER_PLUGIN


#include "../menu_items.hpp"
#include "api_impl/canvas.hpp"


class GaussBlurFilter : public TextButton
{
    friend class GaussBlurFilterAction;
public:
    GaussBlurFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class GaussBlurFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    GaussBlurFilter *button_;
public:
    GaussBlurFilterAction(GaussBlurFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


#endif // GAUSS_BLUR_FILTER_PLUGIN
