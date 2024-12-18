#include "negative.hpp"
#include <cassert>
#include <iostream>


NegativeFilter::NegativeFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                               std::unique_ptr<sfm::IRectangleShape> init_shape)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)) {}


void NegativeFilter::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


std::unique_ptr<IAction> NegativeFilter::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<NegativeFilterAction>(this, renderWindow, event);
}


NegativeFilterAction::NegativeFilterAction(NegativeFilter *button, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool NegativeFilterAction::execute(const Key& key)
{
    button_->updateState(render_window_, event_);
    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }

    Layer *layer = static_cast<Layer *>(canvas_->getLayer( canvas_->getActiveLayerIndex()));
    assert( layer );

    sfm::vec2u canvas_size = canvas_->getSize();
    for ( size_t x = 0; x < canvas_size.x; x++ )
    {
        for ( size_t y = 0; y < canvas_size.y; y++ )
        {
            sfm::Color color = layer->getPixelGlobal( sfm::vec2i( x, y));
            color.r = 255 - color.r;
            color.g = 255 - color.g;
            color.b = 255 - color.b;
            layer->setPixelGlobal( sfm::vec2i( x, y), color);
        }
    }
    button_->setState( IBarButton::State::Normal);

    return true;
}


bool NegativeFilterAction::undo(const Key& key)
{
    std::cout << "Undo negative filter\n";
    return true;
}


bool NegativeFilterAction::redo(const Key& key)
{
    std::cout << "Redo negative filter\n";
    return true;
}


bool NegativeFilterAction::isUndoable(const Key& key)
{
    return false;
}
