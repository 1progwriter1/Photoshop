#include "layer.hpp"
#include <cassert>


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


vec2f ZoomCanvas::setZoom(vec2f zoom)
{
    zoom_ = zoom;
    return zoom_;
}


ZoomCanvasAction::ZoomCanvasAction(ZoomCanvas *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool ZoomCanvasAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);
    if ( button_->state_ != psapi::IBarButton::State::Press )
    {
        return true;
    }
    button_->zoom_.x = std::min(3.0, button_->zoom_.x + 0.1);
    button_->zoom_.y = std::min(3.0, button_->zoom_.y + 0.1);

    button_->another_->setZoom(button_->zoom_);
    canvas_->setZoom(button_->zoom_);
    button_->setState(psapi::IBarButton::State::Normal);

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


vec2f UnZoomCanvas::setZoom(vec2f zoom)
{
    zoom_ = zoom;
    return zoom_;
}


UnZoomCanvasAction::UnZoomCanvasAction(UnZoomCanvas *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool UnZoomCanvasAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);
    if ( button_->state_ != psapi::IBarButton::State::Press )
    {
        return true;
    }
    button_->zoom_.x = std::max(0.1, button_->zoom_.x - 0.1);
    button_->zoom_.y = std::max(0.1, button_->zoom_.y - 0.1);

    button_->another_->setZoom(button_->zoom_);
    canvas_->setZoom(button_->zoom_);
    button_->setState(psapi::IBarButton::State::Normal);

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
    if ( button_->state_ != psapi::IBarButton::State::Press )
    {
        return true;
    }
    getActionController()->redo();
    button_->setState(psapi::IBarButton::State::Normal);
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
    if ( button_->state_ != psapi::IBarButton::State::Press )
    {
        return true;
    }
    getActionController()->undo();
    button_->setState(psapi::IBarButton::State::Normal);
    return true;
}


bool UndoAction::isUndoable(const Key &key)
{
    return false;
}
