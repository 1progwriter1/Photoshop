#include <api_impl/scroll.hpp>
#include <cassert>
#include <iostream>


VerticalScroll::VerticalScroll(vec2i sector_pos, vec2u sector_size, vec2i window_pos, vec2u window_size)
{
    sector_pos_ = sector_pos;
    sector_size_ = sector_size;
    window_size_ = window_size;

    scroll_bar_ = std::make_unique<RectangleShape>();
    scroll_bar_->setPosition( vec2i( sector_pos.x - 20 + sector_size.x, sector_pos.y));
    scroll_bar_->setSize( vec2u( 20, sector_size.y));
    scroll_bar_->setFillColor( Color(192, 192, 192));

    scroll_bar_button_ = std::make_unique<RectangleShape>();
    scroll_bar_button_->setPosition( vec2i( sector_pos.x - 20 + sector_size.x, sector_pos.y));
    scroll_bar_button_->setSize( vec2u( 20, sector_size.y / (static_cast<float>( window_size.y) / static_cast<float>( sector_size.y))));
    scroll_bar_button_->setFillColor( Color( 127, 0, 255));

    ratio_ = static_cast<float>( window_size.y) / static_cast<float>( sector_size.y);
}


HorizontalScroll::HorizontalScroll( vec2i sector_pos, vec2u sector_size, vec2i window_pos, vec2u window_size)
{
    sector_pos_ = sector_pos;
    sector_size_ = sector_size;
    window_size_ = window_size;

    scroll_bar_ = std::make_unique<RectangleShape>();
    scroll_bar_->setPosition( vec2i( sector_pos.x, sector_pos.y));
    scroll_bar_->setSize( vec2u( sector_size.x, 20));
    scroll_bar_->setFillColor( Color(192, 192, 192));

    scroll_bar_button_ = std::make_unique<RectangleShape>();
    scroll_bar_button_->setPosition( vec2i( sector_pos.x, sector_pos.y));
    scroll_bar_button_->setSize( vec2u( sector_size.x / (static_cast<float>( window_size.x) / static_cast<float>( sector_size.x)), 20));
    scroll_bar_button_->setFillColor( Color( 127, 0, 255));

    ratio_ = static_cast<float>( window_size.x) / static_cast<float>( sector_size.x);
}


void Scrollable::draw(psapi::sfm::IRenderWindow *renderWindow) const
{
    renderWindow->draw( static_cast<psapi::sfm::Drawable *>( scroll_bar_.get()));
    renderWindow->draw( static_cast<psapi::sfm::Drawable *>( scroll_bar_button_.get()));
}

int Scrollable::getOffset( const psapi::sfm::IRenderWindow *renderWindow, const Event &event)
{
    return 0;
}


bool VerticalScroll::update(const psapi::sfm::IRenderWindow *renderWindow, const psapi::sfm::Event &event, vec2i &pos)
{
    int offset = getOffset( renderWindow, event);
    if ( offset == 0 )
        return true;

    vec2f button_pos = scroll_bar_button_->getPosition();
    int new_pos = button_pos.y - offset;

    new_pos = std::max<int>( scroll_bar_->getPosition().y, new_pos);
    new_pos = std::min<int>( scroll_bar_->getSize().y + scroll_bar_->getPosition().y - scroll_bar_button_->getSize().y, new_pos);

    scroll_bar_button_->setPosition( vec2f( button_pos.x, new_pos));
    pos.y = -static_cast<float>( new_pos - sector_pos_.y) / sector_size_.y * static_cast<float>( window_size_.y) + sector_pos_.y;

    return true;
}


int VerticalScroll::getOffset( const psapi::sfm::IRenderWindow *renderWindow, const Event &event)
{
    assert( renderWindow );

    vec2i mouse_pos = psapi::sfm::Mouse::getPosition( renderWindow);

    if ( event.type == psapi::sfm::Event::MouseButtonPressed && isOnScrollButton( mouse_pos) )
    {
        last_mouse_pos_ = mouse_pos;
        is_moved_ = true;
        return 0;
    }
    if ( event.type == psapi::sfm::Event::MouseButtonReleased && is_moved_ )
    {
        last_mouse_pos_ = vec2i();
        is_moved_ = false;
        return 0;
    }
    if ( is_moved_ && ( psapi::sfm::Mouse::isButtonPressed( psapi::sfm::Mouse::Button::Left)
                        || psapi::sfm::Mouse::isButtonPressed( psapi::sfm::Mouse::Button::Left) ) )
    {
        int offset = last_mouse_pos_.y - mouse_pos.y;
        last_mouse_pos_ = mouse_pos;
        return offset;
    }
    if ( isOnFocus( mouse_pos) && event.type == psapi::sfm::Event::MouseWheelScrolled )
    {
        if ( event.mouseWheel.wheel == psapi::sfm::Mouse::Wheel::Vertical )
        {
            return event.mouseWheel.delta;
        }
    }

    return 0;
}


bool HorizontalScroll::update(const psapi::sfm::IRenderWindow *renderWindow, const psapi::sfm::Event &event, vec2i &pos)
{
    int offset = getOffset( renderWindow, event);
    if ( offset == 0 )
        return true;

    vec2f button_pos = scroll_bar_button_->getPosition();
    int new_pos = button_pos.x - offset;

    new_pos = std::max<int>( scroll_bar_->getPosition().x, new_pos);
    new_pos = std::min<int>( scroll_bar_->getSize().x + scroll_bar_->getPosition().x - scroll_bar_button_->getSize().x, new_pos);

    scroll_bar_button_->setPosition( vec2f( new_pos, button_pos.y));
    pos.x = -static_cast<float>( new_pos - sector_pos_.x) / sector_size_.x * static_cast<float>( window_size_.x) + sector_pos_.x;

    return true;
}


bool Scrollable::isOnFocus(const vec2i &mouse_pos) const
{
    return sector_pos_.x <= mouse_pos.x && mouse_pos.x < sector_pos_.x + sector_size_.x &&
         sector_pos_.y <= mouse_pos.y && mouse_pos.y < sector_pos_.y + sector_size_.y;
}


int HorizontalScroll::getOffset( const psapi::sfm::IRenderWindow *renderWindow, const Event &event)
{
    assert( renderWindow );

    vec2i mouse_pos = psapi::sfm::Mouse::getPosition( renderWindow);

    if ( event.type == psapi::sfm::Event::MouseButtonPressed && isOnScrollButton( mouse_pos) )
    {
        last_mouse_pos_ = mouse_pos;
        is_moved_ = true;
        return 0;
    }
    if ( event.type == psapi::sfm::Event::MouseButtonReleased  && is_moved_)
    {
        last_mouse_pos_ = vec2i();
        is_moved_ = false;
        return 0;
    }
    if ( is_moved_ && ( psapi::sfm::Mouse::isButtonPressed( psapi::sfm::Mouse::Button::Left)
                        || psapi::sfm::Mouse::isButtonPressed( psapi::sfm::Mouse::Button::Left) ) )
    {
        int offset = last_mouse_pos_.x - mouse_pos.x;
        last_mouse_pos_ = mouse_pos;
        return offset;
    }
    if ( isOnFocus( mouse_pos) && event.type == psapi::sfm::Event::MouseWheelScrolled )
    {
        if ( event.mouseWheel.wheel == psapi::sfm::Mouse::Wheel::Horizontal )
        {
            return event.mouseWheel.delta;
        }
    }

    return 0;
}


bool Scrollable::isOnScrollButton(const vec2i &mouse_pos) const
{
    vec2f button_pos = scroll_bar_button_->getPosition();
    vec2u button_size = scroll_bar_button_->getSize();

    // std::cerr << "Button pos: " << button_pos.x << ' ' << button_pos.y << std::endl;
    // std::cerr << "Button size: " << button_size.x << ' ' << button_size.y << std::endl;
    // std::cerr << "Mouse pos: " << mouse_pos.x << ' ' << mouse_pos.y << std::endl;

    return button_pos.x <= mouse_pos.x && mouse_pos.x < button_pos.x + button_size.x &&
           button_pos.y <= mouse_pos.y && mouse_pos.y < button_pos.y + button_size.y;
}
