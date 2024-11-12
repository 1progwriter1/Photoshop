#include <api_impl/scroll.hpp>
#include <cassert>
#include <iostream>

Scrollable::Scrollable(vec2i sector_pos, vec2u sector_size, vec2i window_pos, vec2u window_size)
    :   sector_pos_( sector_pos), sector_size_( sector_size), cur_pos_( window_pos), window_size_( window_size)
{
    scroll_bar_ = std::make_unique<RectangleShape>();
    scroll_bar_->setPosition( vec2i( sector_pos.x - 20 + sector_size.x, sector_pos.y));
    scroll_bar_->setSize( vec2u( 20, sector_size.y));
    scroll_bar_->setFillColor( Color(192, 192, 192));

    scroll_bar_button_ = std::make_unique<RectangleShape>();
    scroll_bar_button_->setPosition( vec2i( sector_pos.x - 20 + sector_size.x, sector_pos.y));
    scroll_bar_button_->setSize( vec2u( 20, sector_size.y / (static_cast<float>( window_size.y) / static_cast<float>( sector_size.y))));
    scroll_bar_button_->setFillColor( Color( 51, 153, 255));

    ratio_ = static_cast<float>( window_size.y) / static_cast<float>( sector_size.y);

    std::cerr << "Ratio " << ratio_ << '\n';
}


void Scrollable::draw(psapi::sfm::IRenderWindow *renderWindow) const
{
    renderWindow->draw( static_cast<psapi::sfm::Drawable *>( scroll_bar_.get()));
    renderWindow->draw( static_cast<psapi::sfm::Drawable *>( scroll_bar_button_.get()));
}


void Scrollable::updateWindowPosition(vec2i &pos)
{
    pos = cur_pos_;
}


bool Scrollable::update(const psapi::sfm::IRenderWindow *renderWindow, const psapi::sfm::Event &event)
{
    bool on_focus = isOnFocus( psapi::sfm::Mouse::getPosition( renderWindow));
    if ( event.type == psapi::sfm::Event::MouseWheelScrolled && on_focus )
    {
        if ( event.mouseWheel.wheel == psapi::sfm::Mouse::Wheel::Vertical )
        {
            vec2f button_pos = scroll_bar_button_->getPosition();
            int new_pos = button_pos.y - event.mouseWheel.delta;

            new_pos = std::max<int>( scroll_bar_->getPosition().y, new_pos);
            new_pos = std::min<int>( scroll_bar_->getSize().y + scroll_bar_->getPosition().y - scroll_bar_button_->getSize().y, new_pos);

            scroll_bar_button_->setPosition( vec2f( button_pos.x, new_pos));
            cur_pos_ = vec2i( cur_pos_.x, -static_cast<float>( new_pos) / sector_size_.y * static_cast<float>( window_size_.y));
        }
    }
    return true;
}


bool Scrollable::isOnFocus(const vec2i &mouse_pos) const
{
    return sector_pos_.x <= mouse_pos.x && mouse_pos.x < sector_pos_.x + sector_size_.x &&
         sector_pos_.y <= mouse_pos.y && mouse_pos.y < sector_pos_.y + sector_size_.y;
}

