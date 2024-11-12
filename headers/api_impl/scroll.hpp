#ifndef SCROLL_WINDOWS_IMPL
#define SCROLL_WINDOWS_IMPL

#include <api_system.hpp>
#include <api_sfm.hpp>
#include <api_impl/sfm.hpp>

using psapi::sfm::vec2i;
using psapi::sfm::vec2u;
using psapi::sfm::vec2f;
using psapi::sfm::RectangleShape;
using psapi::sfm::Color;
using psapi::sfm::Event;

class Scrollable : psapi::sfm::Drawable
{
    const vec2i sector_pos_;
    const vec2u sector_size_;
    const vec2u window_size_;
    vec2i cur_pos_;

    bool is_moved_ = false;
    vec2i last_mouse_pos_ = vec2i();
    float ratio_;

    std::unique_ptr<RectangleShape> scroll_bar_;
    std::unique_ptr<RectangleShape> scroll_bar_button_;
public:
    Scrollable( vec2i sector_pos, vec2u sector_size, vec2i window_pos, vec2u window_size);
    virtual ~Scrollable() {}
    void updateWindowPosition(vec2i &pos);
    void draw(psapi::sfm::IRenderWindow *renderWindow) const override;
    bool update(const psapi::sfm::IRenderWindow *renderWindow, const psapi::sfm::Event &event);
    bool isOnFocus(const vec2i &mouse_pos) const;
};

#endif // SCROLL_WINDOWS_IMPL
