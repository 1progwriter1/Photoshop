#ifndef FILTERS_FOR_MENU
#define FILTERS_FOR_MENU


#include "menu_items.hpp"
#include "api/api_canvas.hpp"
#include "api_impl/canvas.hpp"


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


sfm::Color createContrastColor( const sfm::Color &normal, const sfm::Color &blurred, int cf);


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


class BrightnessFilter : public TextButton
{
    friend class BrightnessFilterAction;
public:
    BrightnessFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class BrightnessFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    BrightnessFilter *button_;
public:
    BrightnessFilterAction(BrightnessFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


#endif // FILTERS_FOR_MENU
