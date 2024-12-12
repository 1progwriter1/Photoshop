#include "tools.hpp"


Seagfault::Seagfault(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                        std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


std::unique_ptr<IAction> Seagfault::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<SeagfaultAction>(this, renderWindow, event);
}


void Seagfault::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


SeagfaultAction::SeagfaultAction(Seagfault *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button) {}


bool SeagfaultAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);
    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }
    button_ = nullptr;
    button_->updateState(render_window_, event_);
    return true;
}


bool SeagfaultAction::isUndoable(const Key &key)
{
    return false;
}
