#include "barelief.hpp"
#include <iostream>
#include "api_impl/canvas.hpp"
#include <cassert>


BareliefFilter::BareliefFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                               std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


std::unique_ptr<IAction> BareliefFilter::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<BareliefFilterAction>(this, renderWindow, event);
}


void BareliefFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


BareliefFilterAction::BareliefFilterAction(BareliefFilter *button, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool BareliefFilterAction::isUndoable(const Key &key)
{
    return false;
}


bool BareliefFilterAction::undo(const Key &key)
{
    std::cout << "Undo filter\n";
    return true;
}


bool BareliefFilterAction::redo(const Key &key)
{
    std::cout << "Redo filter\n";
    return true;
}


bool BareliefFilterAction::execute(const Key &key)
{
    button_->updateState(render_window_, event_);

    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }

    Layer *layer = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( layer );
    canvas_->insertEmptyLayer( 1);
    Layer *new_layer = static_cast<Layer *>(canvas_->getLayer( 1));
    assert( new_layer );
    assert( new_layer != layer );

    sfm::vec2u canvas_size = canvas_->getSize();
    for ( unsigned x = 0; x < canvas_size.x; x++ )
    {
        for ( unsigned y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color color_1 = layer->getPixelGlobal( sfm::vec2i( std::min( x + 4, canvas_size.x - 1), y));
            sfm::Color color_2 = layer->getPixelGlobal( sfm::vec2i( x, std::min( y + 4, canvas_size.y - 1)));

            sfm::Color color;
            color.r = (255 - color_1.r + color_2.r) / 2;
            color.g = (255 - color_1.g + color_2.g) / 2;
            color.b = (255 - color_1.b + color_2.b) / 2;
            color.a = (255 - color_1.a + color_2.a) / 2;
            new_layer->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }

    canvas_->removeLayer( 0);
    button_->setState( IBarButton::State::Normal);

    return true;
}
