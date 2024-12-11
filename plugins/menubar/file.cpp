#include "file.hpp"
#include <cassert>
#include <iostream>
#include "api_impl/sfm.hpp"


OpenFile::OpenFile(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape, std::string &file)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)), file_(std::move(file))
{}


std::unique_ptr<IAction> OpenFile::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<OpenFileAction>(this, renderWindow, event);
}


void OpenFile::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


OpenFileAction::OpenFileAction(OpenFile *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool OpenFileAction::execute(const Key& key)
{
    button_->updateState(render_window_, event_);
    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }
    std::unique_ptr<sfm::IImage> image = sfm::IImage::create();
    image->loadFromFile(button_->file_);

    vec2u size = image->getSize();
    ILayer *layer = canvas_->getLayer(canvas_->getActiveLayerIndex());
    for ( unsigned int x = 0; x < size.x; x++ )
    {
        for ( unsigned int y = 0; y < size.y; y++ )
        {
            layer->setPixel( sfm::vec2i( x, y), image->getPixel(vec2u(x, y)));
        }
    }
    button_->setState(psapi::IBarButton::State::Normal);
    return true;
}


bool OpenFileAction::isUndoable(const Key& key)
{
    return false;
}


SaveFile::SaveFile(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                    std::unique_ptr<sfm::IRectangleShape> init_shape, std::string &file)
    :   TextButton(init_id, std::move(font), std::move(text), std::move(init_shape)), file_(std::move(file)) {}



std::unique_ptr<IAction> SaveFile::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<SaveFileAction>(this, renderWindow, event);
}


void SaveFile::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(TextButton::createAction(renderWindow, event));
}


SaveFileAction::SaveFileAction(SaveFile *button, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), button_(button)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );
}


bool SaveFileAction::execute(const Key& key)
{
    button_->updateState(render_window_, event_);
    if ( button_->getState() != IBarButton::State::Press )
    {
        return true;
    }
    std::unique_ptr<sfm::Image> image = std::make_unique<sfm::Image>();
    ILayer *layer = canvas_->getLayer(canvas_->getActiveLayerIndex());

    vec2u size = getCanvasIntRect().size - vec2u(20, 20);
    image->create(size.x, size.y);

    for ( unsigned int x = 0; x < size.x; x++ )
    {
        for ( unsigned int y = 0; y < size.y; y++ )
        {
            image->setPixel( sfm::vec2u( x, y), layer->getPixel(vec2u(x, y)));
        }
    }
    image->saveToFile(button_->file_);
    button_->setState(psapi::IBarButton::State::Normal);
    return true;
}


bool SaveFileAction::isUndoable(const Key& key)
{
    return false;
}


