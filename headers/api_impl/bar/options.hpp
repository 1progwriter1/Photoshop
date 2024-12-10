#include <api/api_bar.hpp>


using psapi::IColorPalette;
using psapi::IThicknessOption;
using psapi::IOpacityOption;

using psapi::sfm::Color;

using psapi::sfm::vec2i;
using psapi::sfm::vec2u;


class ColorPalette : public IColorPalette
{
    Color color_ = Color(0, 0, 0);
public:
    ColorPalette(vec2i init_pos, vec2u init_size);

    Color getColor() const override;
    void setColor(const Color &color) override;
};


class ThicknessOption : public IThicknessOption
{
    float thickness_ = 0;
public:
    ThicknessOption(vec2i init_pos, vec2u init_size);

    float getThickness() const override;
    void setThickness(float thickness) override;
};


class OpacityOption : public IOpacityOption
{
    float opacity_ = 0;
public:
    OpacityOption(vec2i init_pos, vec2u init_size);

    float getOpacity() const override;
    void setOpacity(float opacity) override;
};
