#include <api_impl/bar/button.hpp>
#include <cassert>


using psapi::sfm::Mouse;
using psapi::sfm::IntRect;


void ABarButton::draw(IRenderWindow* renderWindow)
{
    renderWindow->draw( sprite_.get());
}


ABarButton::ABarButton(  wid_t init_id, std::unique_ptr<ITexture> &init_texture, std::unique_ptr<ISprite> &init_sprite)
    :   id_( init_id)
{
    texture_ = std::move( init_texture);
    sprite_ = std::move( init_sprite);
}


std::unique_ptr<IAction> ABarButton::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<BarButtonAction>(this, renderWindow, event);
}


wid_t ABarButton::getId() const
{
    return id_;
}


void ABarButton::setSize(const vec2u &size)
{
    sprite_->setTextureRect(IntRect(vec2dToVec2u(sprite_->getPosition()), size));
}


void ABarButton::setPos(const vec2i &pos)
{
    sprite_->setPosition( pos.x, pos.y);
}


IWindow* ABarButton::getWindowById(wid_t id)
{
    if ( id_ == id )
        return this;

    return nullptr;
}


const IWindow* ABarButton::getWindowById(wid_t id) const
{
    if ( id_ == id )
        return this;

    return nullptr;
}


vec2i ABarButton::getPos() const
{
    vec2f pos = sprite_->getPosition();
    return vec2i( static_cast<int>( pos.x), static_cast<int>( pos.y));
}


vec2u ABarButton::getSize() const
{
    return sprite_->getSize();
}


void ABarButton::setParent(const IWindow* parent)
{
    parent_ = dynamic_cast<const IBar *>( parent);
    assert( parent_ );
}


void ABarButton::forceActivate()
{
    assert( 0 && "Not implemented" );
}


void ABarButton::forceDeactivate()
{
    assert( 0 && "Not implemented" );
}


bool ABarButton::isActive() const
{
    assert( 0 && "Not implemented" );

    return false;
}


bool ABarButton::isWindowContainer() const
{
    return false;
}


void ABarButton::setState(State state)
{
    state_ = state;
}


IBarButton::State ABarButton::getState() const
{
    return state_;
}


AMenuButton::AMenuButton(wid_t init_id, std::unique_ptr<IRectangleShape> init_shape, std::unique_ptr<IBar> nested_bar)
    :   id_(init_id), main_shape_(std::move(init_shape)), bar_(std::move(nested_bar)) {}


void AMenuButton::addMenuItem(std::unique_ptr<IWindow> item)
{
    bar_->addWindow(std::move(item));
}


void AMenuButton::activateMenu()
{
    is_bar_active_ = true;
}


void AMenuButton::deactivateMenu()
{
    is_bar_active_ = false;
}


IBar *AMenuButton::getMenu()
{
    return bar_.get();
}


const IBar *AMenuButton::getMenu() const
{
    return bar_.get();
}


void AMenuButton::draw(IRenderWindow* renderWindow)
{
    renderWindow->draw( main_shape_.get());

    if ( state_ == psapi::IBarButton::State::Press )
    {
        is_bar_active_ = true;
    } else
    {
        is_bar_active_ = false;
    }

    if ( is_bar_active_ )
    {
        // bar_->draw(renderWindow);
    }
}


std::unique_ptr<IAction> AMenuButton::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<AMenuButtonAction>(this, renderWindow, event);
}


wid_t AMenuButton::getId() const
{
    return id_;
}


IWindow* AMenuButton::getWindowById(wid_t id)
{
    if ( id_ == id )
        return this;

    return nullptr;
}


const IWindow* AMenuButton::getWindowById(wid_t id) const
{
    if ( id_ == id )
        return this;

    return nullptr;
}


vec2i AMenuButton::getPos() const
{
    vec2f pos = main_shape_->getPosition();
    return vec2i( static_cast<int>( pos.x), static_cast<int>( pos.y));
}


vec2u AMenuButton::getSize() const
{
    return main_shape_->getSize();
}


void AMenuButton::setParent(const IWindow* parent)
{
    parent_ = dynamic_cast<const IBar *>( parent);
    assert( parent_ );
}


void AMenuButton::setPos(const vec2i &pos)
{
    main_shape_->setPosition( pos);
}


void AMenuButton::setSize(const vec2u &size)
{
    main_shape_->setSize(size);
}


void AMenuButton::forceActivate()
{
    assert( 0 && "Not implemented" );
}


void AMenuButton::forceDeactivate()
{
    assert( 0 && "Not implemented" );
}


bool AMenuButton::isActive() const
{
    assert( 0 && "Not implemented" );

    return false;
}


bool AMenuButton::isWindowContainer() const
{
    return false;
}


void AMenuButton::setState(State state)
{
    state_ = state;
}


IBarButton::State AMenuButton::getState() const
{
    return state_;
}



AMenuButtonAction::AMenuButtonAction(AMenuButton *button, const IRenderWindow *render_window, const Event &event)
    :   AAction(render_window, event), button_(button) {}


bool AMenuButtonAction::isUndoable(const Key &key)
{
    return false;
}


bool AMenuButtonAction::execute(const Key &key)
{
    vec2i mouse_pos = Mouse::getPosition(render_window_);
    vec2i button_pos = button_->getPos();

    vec2u size = button_->getSize();
    bool is_on_focus = button_pos.x <= mouse_pos.x && mouse_pos.x <= button_pos.x + size.x &&
                        button_pos.y <= mouse_pos.y && mouse_pos.y <= button_pos.y + size.y;
    if ( is_on_focus )
    {
        if ( event_.type == Event::MouseButtonPressed )
        {
            button_->state_ = (button_->state_ != IBarButton::State::Press) ? IBarButton::State::Press : IBarButton::State::Released;
        } else if ( button_->state_ != IBarButton::State::Press )
        {
            button_->state_ = psapi::IBarButton::State::Hover;
        }
    } else if ( button_->state_ == psapi::IBarButton::State::Hover || button_->state_ == psapi::IBarButton::State::Released )
    {
        button_->state_ = psapi::IBarButton::State::Normal;
    }

    if ( button_->state_ == psapi::IBarButton::State::Press )
    {
        psapi::getActionController()->execute(button_->bar_->createAction(render_window_, event_));
    }
    return true;
}


BarButtonAction::BarButtonAction(ABarButton *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button) {}


bool BarButtonAction::isUndoable(const Key &key)
{
    return false;
}


bool BarButtonAction::execute(const Key &key)
{
    vec2i mouse_pos = Mouse::getPosition(render_window_);
    vec2i button_pos = button_->getPos();

    vec2u size = button_->getSize();
    bool is_on_focus = button_pos.x <= mouse_pos.x && mouse_pos.x <= button_pos.x + size.x &&
                        button_pos.y <= mouse_pos.y && mouse_pos.y <= button_pos.y + size.y;
    if ( is_on_focus )
    {
        if ( event_.type == Event::MouseButtonPressed )
        {
            if ( button_->state_ != IBarButton::State::Press )
            {
                button_->state_ = IBarButton::State::Press;
            } else
            {
                button_->state_ = IBarButton::State::Released;
                static_cast<psapi::IOptionsBar *>(psapi::getRootWindow()->getWindowById(psapi::kOptionsBarWindowId))->removeAllOptions();
            }
        } else if ( button_->state_ != IBarButton::State::Press )
        {
            button_->state_ = psapi::IBarButton::State::Hover;
        }
    } else if ( button_->state_ == psapi::IBarButton::State::Hover || button_->state_ == psapi::IBarButton::State::Released )
    {
        button_->state_ = psapi::IBarButton::State::Normal;
    }
    return true;
}
