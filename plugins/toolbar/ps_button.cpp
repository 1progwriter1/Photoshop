#include "ps_button.hpp"
#include <windows_id.hpp>
#include <cassert>


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool onLoadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    psapi::IBar *toolbar = dynamic_cast<psapi::IBar *>( kRootWindowPtr->getWindowById( psapi::kToolBarWindowId));
    assert( toolbar && "Failed to cast to IBar" );

    std::unique_ptr<sfm::ITexture> texture = sfm::ITexture::create();
    std::unique_ptr<sfm::ISprite> sprite = sfm::ISprite::create();
    texture->loadFromFile("../assets/images/ps32_32.png");
    sprite->setTexture( texture.get());

    std::unique_ptr<ABarButton> brush = std::make_unique<PsButton>( kPsButtonId, std::move(texture), std::move(sprite));
    brush->setParent( toolbar);
    toolbar->addWindow( std::move( brush));

    return true;
}

void onUnloadPlugin()
{

}


PsButton::PsButton( wid_t init_id, std::unique_ptr<sfm::ITexture> init_texture, std::unique_ptr<sfm::ISprite> init_sprite)
    :   ABarButton(init_id, std::move(init_texture), std::move(init_sprite)) {}


void PsButton::updateState(const IRenderWindow *render_window, const Event &event)
{
    getActionController()->execute(ABarButton::createAction(render_window, event));
}


std::unique_ptr<IAction> PsButton::createAction(const IRenderWindow *render_window, const Event &event)
{
    return std::make_unique<PsButtonAction>(this, render_window, event);
}


PsButtonAction::PsButtonAction(PsButton *button, const IRenderWindow *render_window, const Event &event)
    :   AAction(render_window, event), button_(button) {}


bool PsButtonAction::execute(const Key &key)
{
    return true;
}


bool PsButtonAction::isUndoable(const Key &key)
{
    return false;
}

