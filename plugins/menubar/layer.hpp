#ifndef PLUGIN_MENU_LAYER
#define PLUGIN_MENU_LAYER


#include "menu_items.hpp"


class ZoomCanvas : public TextButton
{
    friend class ZoomCanvasAction;
public:
    ZoomCanvas(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
               std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class ZoomCanvasAction : public AAction
{
    ZoomCanvas *button_;
public:
    ZoomCanvasAction(ZoomCanvas *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


class UnZoomCanvas : public TextButton
{
    friend class UnZoomCanvasAction;
public:
    UnZoomCanvas(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                 std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class UnZoomCanvasAction : public AAction
{
    UnZoomCanvas *button_;
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
