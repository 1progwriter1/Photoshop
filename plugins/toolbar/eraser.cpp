#include "eraser.hpp"
#include <cassert>
#include <iostream>
#include "../headers/windows_id.hpp"
#include "api_impl/canvas.hpp"


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool onLoadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    psapi::IBar *toolbar = dynamic_cast<psapi::IBar *>( kRootWindowPtr->getWindowById( psapi::kToolBarWindowId));
    assert( toolbar && "Failed to cast to IBar" );

    std::unique_ptr<sfm::ITexture> texture = std::make_unique<sfm::Texture>();
    std::unique_ptr<sfm::ISprite> sprite = std::make_unique<sfm::Sprite>();
    texture->loadFromFile("../assets/images/eraser48_48.png");
    sprite->setTexture( texture.get());

    std::unique_ptr<ABarButton> eraser = std::make_unique<Eraser>( kEraserButtonId, texture, sprite);
    eraser->setParent( toolbar);
    toolbar->addWindow( std::move( eraser));

    return true;
}

void onUnloadPlugin()
{

}


Eraser::Eraser( wid_t init_id, std::unique_ptr<sfm::ITexture> &init_texture, std::unique_ptr<sfm::ISprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite),
        canvas_( dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
}


std::unique_ptr<IAction> Eraser::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<EraserAction>(this, renderWindow, event);
}


void Eraser::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw(renderWindow);
}


void Eraser::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(ABarButton::createAction(renderWindow, event));
}


EraserAction::EraserAction(Eraser *eraser, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), eraser_(eraser) {}


bool EraserAction::isUndoable(const Key &key)
{
    return false;
}


bool EraserAction::redo(const Key &key)
{
    std::cout << "Redo eraser\n";
    return true;
}


bool EraserAction::undo(const Key &key)
{
    std::cout << "Undo eraser\n";
    return true;
}


bool EraserAction::execute(const Key &key)
{
    eraser_->updateState(render_window_, event_);

    if ( eraser_->getState() != psapi::IBarButton::State::Press )
        return true;

    sfm::vec2u size = eraser_->canvas_->getSize();
    Layer *cur_layer = static_cast<Layer *>(eraser_->canvas_->getLayer( eraser_->canvas_->getActiveLayerIndex()));
    assert( cur_layer );

    for ( unsigned int x = 0; x < size.x; x++ )
    {
        for ( unsigned int y = 0; y < size.y; y++ )
        {
            cur_layer->setPixelGlobal( vec2i( x, y), sfm::Color( 255, 255, 255, 255));
        }
    }
    eraser_->setState(psapi::IBarButton::State::Normal);

    return true;
}
