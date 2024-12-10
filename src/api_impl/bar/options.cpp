#include <api_impl/bar/options.hpp>


Color ColorPalette::getColor() const
{
    return color_;
}


void ColorPalette::setColor(const Color &color)
{
    color_ = color;
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
