#include "layer.hpp"


ZoomCanvas::ZoomCanvas(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
    std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


std::unique_ptr<IAction> ZoomCanvas::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<ZoomCanvasAction>(this, renderWindow, event);
}


void ZoomCanvas::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


ZoomCanvasAction::ZoomCanvasAction(ZoomCanvas *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button) {}


bool ZoomCanvasAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);
    return true;
}


bool ZoomCanvasAction::isUndoable(const Key &key)
{
    return false;
}


UnZoomCanvas::UnZoomCanvas(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
    std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


std::unique_ptr<IAction> UnZoomCanvas::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<UnZoomCanvasAction>(this, renderWindow, event);
}


void UnZoomCanvas::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


UnZoomCanvasAction::UnZoomCanvasAction(UnZoomCanvas *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button) {}


bool UnZoomCanvasAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);
    return true;
}


bool UnZoomCanvasAction::isUndoable(const Key &key)
{
    return false;
}


Redo::Redo(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
    std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


std::unique_ptr<IAction> Redo::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<RedoAction>(this, renderWindow, event);
}


void Redo::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


RedoAction::RedoAction(Redo *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button) {}


bool RedoAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);
    return true;
}


bool RedoAction::isUndoable(const Key &key)
{
    return false;
}


Undo::Undo(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
    std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


std::unique_ptr<IAction> Undo::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<UndoAction>(this, renderWindow, event);
}


void Undo::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


UndoAction::UndoAction(Undo *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button) {}


bool UndoAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);
    return true;
}


bool UndoAction::isUndoable(const Key &key)
{
    return false;
}
