#include "line.hpp"
#include <cassert>
#include <iostream>
#include "../headers/windows_id.hpp"


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool onLoadPlugin()
{
    kRootWindowPtr = psapi::getRootWindow();

    psapi::IBar *toolbar = dynamic_cast<psapi::IBar *>( kRootWindowPtr->getWindowById( psapi::kToolBarWindowId));
    assert( toolbar && "Failed to cast to IBar" );

    std::unique_ptr<sfm::ITexture> texture = std::make_unique<sfm::Texture>();
    std::unique_ptr<sfm::ISprite> sprite = std::make_unique<sfm::Sprite>();
    texture->loadFromFile("../assets/images/line48_48.png");
    sprite->setTexture( texture.get());

    std::unique_ptr<ABarButton> line = std::make_unique<Line>( kLineButtonId, texture, sprite);
    line->setParent( toolbar);
    toolbar->addWindow( std::move( line));

    return true;
}

void onUnloadPlugin()
{

}


Line::Line( wid_t init_id, std::unique_ptr<sfm::ITexture> &init_texture, std::unique_ptr<sfm::ISprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite), canvas_( dynamic_cast<ICanvas *>( kRootWindowPtr->getWindowById( psapi::kCanvasWindowId)))
{
    assert( canvas_ && "Failed to cast to canvas" );
    layer_ = canvas_->getTempLayer();
    assert( layer_ && "Failed to get temp layer" );

    canvas_rect_ = psapi::getCanvasIntRect();
    canvas_rect_.pos += vec2i(0, 20);
    canvas_rect_.size -= vec2u(20, 20);
}


void Line::draw( sfm::IRenderWindow *renderWindow)
{
    ABarButton::draw( renderWindow);
    parent_->finishButtonDraw( renderWindow, this);
}


void Line::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(ABarButton::createAction(renderWindow, event));
}


std::unique_ptr<IAction> Line::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<LineAction>(this, renderWindow, event);
}


void Line::setAngleAndLength( sfm::vec2i begin_pos, sfm::vec2i end_pos)
{
    shape_.setPosition( begin_pos);
    shape_.setRotation( std::atan2( end_pos.y - begin_pos.y, end_pos.x - begin_pos.x) * 180 / M_PI);
    shape_.setSize( sfm::vec2u( len( begin_pos, end_pos), 5));
}


bool Line::isOnCanvas( sfm::vec2i mouse_pos)
{
    return canvas_rect_.pos.x <= mouse_pos.x && mouse_pos.x < canvas_rect_.pos.x + canvas_rect_.size.x &&
           canvas_rect_.pos.y <= mouse_pos.y && mouse_pos.y < canvas_rect_.pos.y + canvas_rect_.size.y;
}


void Line::addOptions()
{
    for ( auto &option : options_ )
    {
        static_cast<IOptionsBar *>(psapi::getRootWindow()->getWindowById(kOptionsBarWindowId))->addWindow(std::move(option));
    }
    options_.clear();
    options_added_ = true;
}


void Line::removeOptions()
{
    static_cast<IOptionsBar *>(psapi::getRootWindow()->getWindowById(kOptionsBarWindowId))->removeAllOptions();
    options_added_ = false;
}



void Line::createOptions()
{
    std::unique_ptr<IColorPalette> palette = IColorPalette::create();
    palette->setColor(color_);
    palette_ = palette.get();
    options_.push_back(std::move(palette));
}


void Line::drawLine( const sfm::IRenderWindow *renderWindow, ILayer *layer, bool is_temp_layer)
{
    if ( !is_temp_layer )
    {
        canvas_->cleanTempLayer();
    }
    sfm::vec2i end_pos = sfm::Mouse::getPosition( renderWindow) - canvas_rect_.pos;

    setAngleAndLength( begin_pos_, end_pos);

    const sfm::IImage *image = shape_.getImage();

    for ( size_t x = 0; x < canvas_rect_.size.x; x++ )
    {
        for ( size_t y = 0; y < canvas_rect_.size.y; y++ )
        {
            sfm::Color color = image->getPixel( x, y);
            if ( color.a != 0 )
            {
                layer->setPixel( sfm::vec2i( x, y), color_);
            } else if ( is_temp_layer )
            {
                layer->setPixel( sfm::vec2i( x, y), sfm::Color( 0, 0, 0, 0));
            }
        }
    }
}


LineAction::LineAction(Line *line, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), line_(line) {}


bool LineAction::isUndoable(const Key &key)
{
    return false;
}


bool LineAction::redo(const Key &key)
{
    std::cout << "Redo line\n";
    return true;
}


bool LineAction::undo(const Key &key)
{
    std::cout << "Undo line\n";
    return true;
}


bool LineAction::execute(const Key &key)
{
    line_->updateState(render_window_, event_);

    if ( line_->getState() != IBarButton::State::Press )
    {
        line_->options_added_ = false;
        return true;
    }
    if ( !line_->options_added_ )
    {
        line_->createOptions();
        line_->addOptions();
    }
    line_->color_ = line_->palette_->getColor();
    static bool is_front = false;
    if ( event_.type == sfm::Event::None )
    {
        is_front = true;
    }
    if ( event_.type == sfm::Event::MouseButtonPressed && line_->isOnCanvas( sfm::Mouse::getPosition( render_window_)) && is_front )
    {
        is_front = false;
        line_->draw_ = true;
        line_->begin_pos_ = sfm::Mouse::getPosition( render_window_) - line_->canvas_rect_.pos;
        line_->last_mouse_pos_ = line_->begin_pos_;
    } else if ( event_.type == sfm::Event::MouseButtonReleased && line_->draw_ )
    {
        line_->drawLine( render_window_, line_->canvas_->getLayer( line_->canvas_->getActiveLayerIndex()), false);
        line_->draw_ = false;
        line_->begin_pos_ = sfm::vec2i();
        return true;
    }

    if ( line_->draw_ && is_front )
    {
        is_front = false;
        sfm::vec2i new_mouse_pos = sfm::Mouse::getPosition( render_window_) - line_->canvas_->getPos();
        line_->last_mouse_pos_ = new_mouse_pos;
        line_->drawLine( render_window_, line_->layer_, true);
    }
    return true;
}
