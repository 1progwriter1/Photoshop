#include "ellipse.hpp"
#include <cassert>
#include <iostream>
#include "windows_id.hpp"


psapi::IWindowContainer *kRootWindowPtr = nullptr;


bool onLoadPlugin()
{

    kRootWindowPtr = psapi::getRootWindow();

    psapi::IBar *toolbar = dynamic_cast<psapi::IBar *>( kRootWindowPtr->getWindowById( psapi::kToolBarWindowId));
    assert( toolbar && "Failed to cast to IBar" );

    std::unique_ptr<sfm::Texture> texture = std::make_unique<sfm::Texture>();
    std::unique_ptr<sfm::Sprite> sprite = std::make_unique<sfm::Sprite>();
    texture->loadFromFile("../assets/images/ellipse48_48.png");
    sprite->setTexture( texture.get());

    std::unique_ptr<ABarButton> ellipse = std::make_unique<Ellipse>( kEllipseButtonId, texture, sprite);
    toolbar->addWindow( std::move(ellipse));

    return true;
}

void onUnloadPlugin()
{

}


Ellipse::Ellipse( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite)
    :   ABarButton( init_id, init_texture, init_sprite)
{
    canvas_ = dynamic_cast<ICanvas *>( getRootWindow()->getWindowById( kCanvasWindowId));
    assert( canvas_ && "Failed to cast to canvas" );

    layer_ = canvas_->getTempLayer();
    assert( layer_ && "Failed to get temp layer" );

    canvas_rect_ = psapi::getCanvasIntRect();
    canvas_rect_.pos += vec2i(0, 20);
    canvas_rect_.size -= vec2u(20, 20);
}


void Ellipse::drawEllipse( const sfm::IRenderWindow *renderWindow, ILayer *layer, bool is_temp_layer)
{
    assert( renderWindow );
    assert( layer );

    sfm::vec2i mouse_pos = sfm::Mouse::getPosition( renderWindow) - canvas_rect_.pos;
    sfm::vec2u size( std::abs( mouse_pos.x - left_upper_edge_.x), std::abs( mouse_pos.y - left_upper_edge_.y));
    sfm::vec2u size2 = size * size;

    int center_x = std::min( mouse_pos.x, left_upper_edge_.x) + size.x / 2;
    int center_y = std::min( mouse_pos.y, left_upper_edge_.y) + size.y / 2;

    for ( int x = 0; x < canvas_rect_.size.x; x++ )
    {
        for ( int y = 0; y < canvas_rect_.size.y; y++ )
        {
            if ( isOnEllipse( sfm::vec2i( x, y), size2, sfm::vec2i( center_x, center_y)) )
            {
                layer->setPixel( sfm::vec2i( x, y), color_);
            } else if ( is_temp_layer )
            {
                layer->setPixel( sfm::vec2i( x, y), sfm::Color(0, 0, 0, 0));
            }
        }
    }
}


void Ellipse::updateState(const IRenderWindow *renderWindow, const Event &event)
{
    getActionController()->execute(ABarButton::createAction(renderWindow, event));
}


std::unique_ptr<IAction> Ellipse::createAction(const IRenderWindow *renderWindow, const Event &event)
{
    return std::make_unique<EllipseAction>(this, renderWindow, event);
}


bool Ellipse::isOnCanvas( sfm::vec2i mouse_pos)
{
    return canvas_rect_.pos.x <= mouse_pos.x && mouse_pos.x < canvas_rect_.pos.x + canvas_rect_.size.x &&
           canvas_rect_.pos.y <= mouse_pos.y && mouse_pos.y < canvas_rect_.pos.y + canvas_rect_.size.y;
}

bool Ellipse::isOnEllipse( sfm::vec2i pos, sfm::vec2u size2, sfm::vec2i center)
{
    sfm::vec2f delta = sfm::vec2f( pos.x, pos.y) - sfm::vec2f( center.x, center.y);
    float delta_x = delta.x;
    float delta_y = delta.y;
    sfm::vec2f size2_f = sfm::vec2f( size2.x, size2.y);
    sfm::vec2f center_f = sfm::vec2f( center.x, center.y);

    return delta_x * delta_x / size2_f.x + delta_y * delta_y / size2_f.y <= 0.25;
}


EllipseAction::EllipseAction(Ellipse *ellipse, const IRenderWindow *renderWindow, const Event &event)
    :   AUndoableAction(renderWindow, event), ellipse_(ellipse) {}


bool EllipseAction::isUndoable(const Key &key)
{
    return false;
}


bool EllipseAction::redo(const Key &key)
{
    std::cout << "Redo ellipse\n";
    return true;
}


bool EllipseAction::undo(const Key &key)
{
    std::cout << "Undo ellipse\n";
    return true;
}


void Ellipse::addOptions()
{
    for ( auto &option : options_ )
    {
        static_cast<IOptionsBar *>(psapi::getRootWindow()->getWindowById(kOptionsBarWindowId))->addWindow(std::move(option));
    }
    options_.clear();
    options_added_ = true;
}


void Ellipse::removeOptions()
{
    static_cast<IOptionsBar *>(psapi::getRootWindow()->getWindowById(kOptionsBarWindowId))->removeAllOptions();
    options_added_ = false;
}



void Ellipse::createOptions()
{
    std::unique_ptr<IColorPalette> palette = IColorPalette::create();
    palette->setColor(color_);
    palette_ = palette.get();
    options_.push_back(std::move(palette));
}


bool EllipseAction::execute(const Key &key)
{
    ellipse_->updateState(render_window_, event_);

    if ( ellipse_->state_ != psapi::IBarButton::State::Press )
    {
        return true;
    }
    if ( ellipse_->getState() != IBarButton::State::Press )
    {
        ellipse_->options_added_ = false;
        return true;
    }
    if ( !ellipse_->options_added_ )
    {
        ellipse_->createOptions();
        ellipse_->addOptions();
    }
    ellipse_->color_ = ellipse_->palette_->getColor();
    static bool is_front = false;
    if ( event_.type == sfm::Event::None )
    {
        is_front = true;
    }
    if ( event_.type == sfm::Event::MouseButtonPressed && ellipse_->isOnCanvas( sfm::Mouse::getPosition( render_window_)) && is_front )
    {
        is_front = false;
        ellipse_->draw_ = true;
        ellipse_->left_upper_edge_ = sfm::Mouse::getPosition(render_window_) - ellipse_->canvas_rect_.pos;
        ellipse_->last_mouse_pos_ = ellipse_->left_upper_edge_;
    } else if ( event_.type == sfm::Event::MouseButtonReleased && ellipse_->draw_ )
    {
        ellipse_->canvas_->cleanTempLayer();
        ellipse_->drawEllipse( render_window_, ellipse_->canvas_->getLayer( ellipse_->canvas_->getActiveLayerIndex()), false);
        ellipse_->draw_ = false;
        ellipse_->left_upper_edge_ = {};
        return true;
    }
    if ( ellipse_->draw_ && is_front )
    {
        is_front = false;
        sfm::vec2i new_mouse_pos = sfm::Mouse::getPosition(render_window_) - ellipse_->canvas_rect_.pos;
        ellipse_->last_mouse_pos_ = new_mouse_pos;
        ellipse_->drawEllipse( render_window_, ellipse_->layer_, true);
    }
    return true;
}
