#ifndef PLUGIN_MENU_LAYER
#define PLUGIN_MENU_LAYER


#include "api/api_canvas.hpp"
#include "menu_items.hpp"


class UnZoomCanvas;


class ZoomCanvas : public TextButton
{
    vec2f zoom_ = vec2f( 1, 1);
    UnZoomCanvas *another_ = nullptr;

    friend class ZoomCanvasAction;
    friend std::pair<std::unique_ptr<IWindow>, std::unique_ptr<IWindow>> createZoomButtons();
public:
    ZoomCanvas(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
               std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
    vec2f setZoom(vec2f zoom);
};


class ZoomCanvasAction : public AAction
{
    ZoomCanvas *button_;
    ICanvas *canvas_;
public:
    ZoomCanvasAction(ZoomCanvas *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


class UnZoomCanvas : public TextButton
{
    vec2f zoom_ = vec2f( 1, 1);
    ZoomCanvas *another_ = nullptr;

    friend class UnZoomCanvasAction;
    friend std::pair<std::unique_ptr<IWindow>, std::unique_ptr<IWindow>> createZoomButtons();
public:
    UnZoomCanvas(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                 std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
    vec2f setZoom(vec2f zoom);
};


class UnZoomCanvasAction : public AAction
{
    UnZoomCanvas *button_;
    ICanvas *canvas_;
public:
    UnZoomCanvasAction(UnZoomCanvas *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


class Undo : public TextButton
{
    friend class UndoAction;
public:
    Undo(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
         std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class UndoAction : public AAction
{
    Undo *button_;
public:
    UndoAction(Undo *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


class Redo : public TextButton
{
    friend class RedoAction;
public:
    Redo(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
         std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class RedoAction : public AAction
{
    Redo *button_;
public:
    RedoAction(Redo *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};

#endif // PLUGIN_MENU_LAYER
