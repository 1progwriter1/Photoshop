#include <api_impl/bar/options.hpp>
#include <api_impl/sfm.hpp>
#include <cassert>
#include <iostream>


const std::array<Color, 8> PALETTE_COLORS =
{
    Color::getStandardColor(psapi::sfm::Color::Type::Red),
    Color::getStandardColor(psapi::sfm::Color::Type::Green),
    Color::getStandardColor(psapi::sfm::Color::Type::Blue),
    Color::getStandardColor(psapi::sfm::Color::Type::Yellow),
    Color::getStandardColor(psapi::sfm::Color::Type::Cyan),
    Color::getStandardColor(psapi::sfm::Color::Type::Magenta),
    Color::getStandardColor(psapi::sfm::Color::Type::White),
    Color::getStandardColor(psapi::sfm::Color::Type::Black)
};


std::unique_ptr<psapi::IColorPalette> IColorPalette::create()
{
    return std::make_unique<ColorPalette>(vec2i(), vec2u(100, 250));
}


ColorPalette::ColorPalette(vec2i init_pos, vec2u init_size)
    :   pos_( init_pos), size_( init_size)
{
    indicator_ = std::make_unique<RectangleShape>();
    indicator_->setFillColor(Color::getStandardColor(psapi::sfm::Color::Type::Transparent));

    std::unique_ptr<RectangleShape> button = nullptr;
    for ( auto &color : PALETTE_COLORS )
    {
        button = std::make_unique<RectangleShape>();
        button->setFillColor(color);
        colors_.push_back(std::move(button));
    }
}


Color ColorPalette::getColor() const
{
    return color_;
}


void ColorPalette::setColor(const Color &color)
{
    color_ = color;
    indicator_->setFillColor(color_);
}


void ColorPalette::draw(IRenderWindow* renderWindow)
{
    indicator_->draw(renderWindow);
    for ( auto &color : colors_ )
    {
        color->draw(renderWindow);
    }
}


std::unique_ptr<IAction> ColorPalette::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return std::make_unique<ColorPaletteAction>(this, renderWindow, event);
}


wid_t ColorPalette::getId() const
{
    return psapi::kColorPaletteId;
}


psapi::IWindow* ColorPalette::getWindowById(wid_t id)
{
    if ( id == psapi::kColorPaletteId )
        return this;

    return nullptr;
}


const psapi::IWindow* ColorPalette::getWindowById(wid_t id) const
{
    if ( id == psapi::kColorPaletteId )
        return this;

    return nullptr;
}


vec2i ColorPalette::getPos() const
{
    return pos_;
}


vec2u ColorPalette::getSize() const
{
    return size_;
}


void ColorPalette::setSize(const vec2u& size)
{
    size_ = size;
    vec2u button_size = vec2u(size_.x / 2, size_.y / 5);
    indicator_->setSize(vec2u(size_.x, button_size.y));
    vec2i row_pos = vec2i(indicator_->getPosition().x, indicator_->getPosition().y);
    for ( int i = 0; i < 4; i++ )
    {
        row_pos += vec2i(0, button_size.y);
        vec2i button_pos = row_pos;
        for ( int j = 0; j < 2; j++ )
        {
            colors_[i * 2 + j]->setSize(button_size);
            colors_[i * 2 + j]->setPosition(button_pos);
            button_pos += vec2i(button_size.x, 0);
        }
    }
}


void ColorPalette::setPos(const vec2i& pos)
{
    vec2i diff = pos - pos_;
    pos_ = pos;
    for ( auto &color : colors_ )
    {
        color->setPosition(vec2i(color->getPosition().x, color->getPosition().y) + diff);
    }
    indicator_->setPosition(vec2i(indicator_->getPosition().x, indicator_->getPosition().y) + diff);
}


void ColorPalette::setParent(const IWindow* parent)
{
    parent_ = dynamic_cast<const psapi::IOptionsBar *>(parent);
    assert( parent_ && "parent is not IOptionsBar" );
}


void ColorPalette::forceActivate()
{
    assert( 0 && "Not implemented" );
}


void ColorPalette::forceDeactivate()
{
    assert( 0 && "Not implemented" );
}


bool ColorPalette::isActive() const
{
    assert( 0 && "Not implemented" );

    return false;
}


bool ColorPalette::isWindowContainer() const
{
    return false;
}


float ThicknessOption::getThickness() const
{
    return thickness_;
}


void ThicknessOption::setThickness(float thickness)
{
    thickness_ = thickness;
}


float OpacityOption::getOpacity() const
{
    return opacity_;
}


void OpacityOption::setOpacity(float opacity)
{
    opacity_ = opacity;
}


ColorPaletteAction::ColorPaletteAction(ColorPalette *palette, const IRenderWindow *renderWindow, const Event &event)
    :   AAction(renderWindow, event), palette_(palette) {}


bool ColorPaletteAction::isUndoable(const Key &key)
{
    return false;
}


bool ColorPaletteAction::execute(const Key &key)
{
    if ( event_.type != Event::MouseButtonPressed )
    {
        return true;
    }
    vec2i mouse_pos = psapi::sfm::Mouse::getPosition(render_window_);
    for ( auto &color : palette_->colors_ )
    {
        if ( isOnShape(mouse_pos, color) )
        {
            palette_->setColor(color->getFillColor());
            return true;
        }
    }
    return true;
}


bool ColorPaletteAction::isOnShape(const vec2i &mouse_pos, const std::unique_ptr<RectangleShape> &shape)
{
    return mouse_pos.x >= shape->getPosition().x && mouse_pos.x <= shape->getPosition().x + shape->getSize().x &&
           mouse_pos.y >= shape->getPosition().y && mouse_pos.y <= shape->getPosition().y + shape->getSize().y;
}
