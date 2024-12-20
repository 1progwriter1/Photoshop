#include "control_panel.hpp"
#include "fill_menu.hpp"
#include "windows_id.hpp"
#include <cassert>


ControlPanel::ControlPanel()
{
    std::unique_ptr<IWindow> button_ok_tmp = createMenuButton<TextButton>(kOkButtonId, "Ok");
    std::unique_ptr<IWindow> button_cancel_tmp = createMenuButton<TextButton>(kCancelButtonId, "Cancel");
    button_ok_ = std::unique_ptr<TextButton>(static_cast<TextButton *>(button_ok_tmp.release()));
    button_cancel_ = std::unique_ptr<TextButton>(static_cast<TextButton *>(button_cancel_tmp.release()));

    button_ok_->setSize(buttons_rect_size_);
    button_cancel_->setSize(buttons_rect_size_);
    min_buttons_width_ = std::max(button_ok_->getSize().x, button_cancel_->getSize().x) * 2;
    size_.x = min_buttons_width_;

    button_ok_->setPos(vec2i(0, 32));
    button_cancel_->setPos(vec2i(button_ok_->getSize().x, 32));

    shape_->setFillColor(sfm::Color(128, 128, 128));
    shape_->setOutlineColor(sfm::Color(64, 64, 64));
    shape_->setOutlineThickness(1);
    shape_->setSize(size_);
    shape_->setPosition(vec2i(0, 0));

    normal_->setFillColor(sfm::Color(128, 128, 128));
    onHover_->setFillColor(sfm::Color(64, 64, 64));
    pressed_->setFillColor(sfm::Color(51, 255, 153));
    released_->setFillColor(sfm::Color(128, 128, 128));
}


void ControlPanel::draw(IRenderWindow* renderWindow)
{
    if ( !active_ )
        return;
    shape_->draw(renderWindow);
    finishButtonDraw(renderWindow, button_ok_.get());
    finishButtonDraw(renderWindow, button_cancel_.get());
    button_ok_->draw(renderWindow);
    button_cancel_->draw(renderWindow);
}


void ControlPanel::finishButtonDraw(IRenderWindow* renderWindow, const TextButton* button)
{
    vec2i pos = button->getPos();
    vec2u size = button->getSize();
    size.x = getSize().x / 2;
    pos.x = (pos.x < (getPos().x + getSize().x / 2)) ? 0 : size.x;
    pos.x += getPos().x;
    switch ( button->getState() )
    {
        case IBarButton::State::Normal:
            normal_->setPosition( pos);
            normal_->setSize(size);
            renderWindow->draw( normal_.get());
            break;
        case IBarButton::State::Hover:
            onHover_->setPosition( pos);
            onHover_->setSize(size);
            renderWindow->draw( onHover_.get());
            break;
        case IBarButton::State::Press:
            pressed_->setPosition( pos);
            pressed_->setSize(size);
            renderWindow->draw( pressed_.get());
            break;
        case IBarButton::State::Released:
            released_->setPosition( pos);
            released_->setSize(size);
            renderWindow->draw( released_.get());
            break;
        default:
            assert( 0 && "Unreachable" );

    }
}


std::unique_ptr<IAction> ControlPanel::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<ControlPanelAction>(this, renderWindow, event);
}


wid_t ControlPanel::getId() const
{
    return kControlPanelId;
}


IWindow* ControlPanel::getWindowById(wid_t id)
{
    if ( id == kControlPanelId )
        return this;
    if ( id == kOkButtonId )
        return button_ok_.get();
    if ( id == kCancelButtonId )
        return button_cancel_.get();
    return nullptr;
}


const IWindow* ControlPanel::getWindowById(wid_t id) const
{
    if ( id == kControlPanelId )
        return this;
    if ( id == kOkButtonId )
        return button_ok_.get();
    if ( id == kCancelButtonId )
        return button_cancel_.get();
    return nullptr;
}


vec2i ControlPanel::getPos() const
{
    return pos_;
}


vec2u ControlPanel::getSize() const
{
    return size_;
}


void ControlPanel::setSize(const vec2u& size)
{
    int buttons_width = std::max(min_buttons_width_, size.x);
    int buttons_height = std::max(buttons_rect_size_.y, size.y);

    vec2i button_ok_pos = button_ok_->getPos();
    vec2i button_cancel_pos = button_cancel_->getPos();
    vec2u button_ok_size = button_ok_->getSize();
    vec2u button_cancel_size = button_cancel_->getSize();

    int button_ok_offset = (buttons_width / 2 - button_ok_size.x) / 2;
    int button_cancel_offset = (buttons_width / 2 - button_cancel_size.x) / 2;

    button_ok_pos.x = button_ok_offset;
    button_ok_pos.y = buttons_height - button_ok_size.y;
    button_cancel_pos.x = button_cancel_offset + buttons_width / 2;
    button_cancel_pos.y = buttons_height - button_cancel_size.y;

    button_ok_->setPos(button_ok_pos);
    button_cancel_->setPos(button_cancel_pos);

    size_ = vec2u(buttons_width, buttons_height);
    shape_->setSize(size_);

    setPos(getCanvasIntRect().pos + vec2i(getCanvasIntRect().size) - vec2i(size_) - vec2i(5, 5));
}


void ControlPanel::setPos(const vec2i& pos)
{
    vec2i diff = pos - pos_;
    pos_ = pos;
    button_ok_->setPos(button_ok_->getPos() + diff);
    button_cancel_->setPos(button_cancel_->getPos() + diff);
    shape_->setPosition(vec2i(shape_->getPosition().x, shape_->getPosition().y) + diff);
}


void ControlPanel::setParent(const IWindow* parent)
{
    assert( 0 && "I don't know who should be parent for this window" );
}


void ControlPanel::forceActivate()
{
    active_ = true;
}


void ControlPanel::forceDeactivate()
{
    active_ = false;
}


bool ControlPanel::isActive() const
{
    return active_;
}


bool ControlPanel::isWindowContainer() const
{
    return false;
}


int ControlPanel::getButtonsRectHeight() const
{
    return buttons_rect_size_.y;
}


vec2u ControlPanel::fitButtonsIn(vec2u size)
{
    unsigned int buttons_width = std::max(min_buttons_width_, size.x);
    unsigned int buttons_height = std::max<unsigned int>(50, size.y);

    vec2i button_ok_pos = button_ok_->getPos();
    vec2i button_cancel_pos = button_cancel_->getPos();
    vec2u button_ok_size = button_ok_->getSize();
    vec2u button_cancel_size = button_cancel_->getSize();

    int button_ok_offset = buttons_width / 2 - button_ok_size.x / 2;
    int button_cancel_offset = buttons_width / 2 - button_cancel_size.x / 2;

    button_ok_pos.x = button_ok_offset;
    button_ok_pos.y += buttons_height - size_.x;
    button_cancel_pos.x = button_cancel_offset + buttons_width / 2;
    button_cancel_pos.y += buttons_height - size_.x;

    button_ok_->setPos(button_ok_pos);
    button_cancel_->setPos(button_cancel_pos);

    return vec2u(buttons_width, buttons_height);
}


ABarButton::State ControlPanel::getOkState() const
{
    return button_ok_->getState();
}


ABarButton::State ControlPanel::getCancelState() const
{
    return button_cancel_->getState();
}


void ControlPanel::unsetAll()
{
    button_ok_->setState(psapi::IBarButton::State::Normal);
    button_cancel_->setState(psapi::IBarButton::State::Normal);
}


ControlPanelAction::ControlPanelAction(ControlPanel *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button) {}


bool ControlPanelAction::execute(const Key &key)
{
    getActionController()->execute(button_->button_ok_->createAction(render_window_, event_));
    if ( button_->button_ok_->getState() == psapi::IBarButton::State::Press )
    {
        button_->button_cancel_->setState(psapi::IBarButton::State::Normal);
    }
    getActionController()->execute(button_->button_cancel_->createAction(render_window_, event_));
    if ( button_->button_cancel_->getState() == psapi::IBarButton::State::Press )
    {
        button_->button_ok_->setState(psapi::IBarButton::State::Normal);
    }
    return true;
}


bool ControlPanelAction::isUndoable(const Key &key)
{
    return false;
}

