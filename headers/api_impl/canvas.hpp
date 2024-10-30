#ifndef PHOTOSHOP_CANVAS_IMPLEMENTATION
#define PHOTOSHOP_CANVAS_IMPLEMENTATION


#include <api_canvas.hpp>


using namespace psapi;

using sfm::Color;



class Layer : public ILayer
{
    vec2i pos_;
    vec2i size_;

    std::vector<Color> pixels_;
public:
    Layer( vec2i size, vec2i pos = vec2i());
    ~Layer() = default;

    Color getPixel(sfm::vec2i pos) const override;
    void setPixel(sfm::vec2i pos, sfm::Color pixel) override;
};


class Canvas : public ICanvas
{
    std::vector<std::unique_ptr<ILayer>> layers_;
public:
    Canvas() = default;
    ~Canvas() = default;

    ILayer*       getLayer(size_t index)       override;
    const ILayer* getLayer(size_t index) const override;

    ILayer*       getTempLayer()       override;
    const ILayer* getTempLayer() const override;

    void cleanTempLayer() override;

    size_t getNumLayers() const override;

    size_t getActiveLayerIndex()       const override;
    void   setActiveLayerIndex(size_t index) override;

    bool insertLayer     (size_t index, std::unique_ptr<ILayer> layer) override;
    bool removeLayer     (size_t index) override;
    bool insertEmptyLayer(size_t index) override;

    void setPos  (sfm::vec2i pos)   override;
    void setSize (sfm::vec2i size)  override;
    void setScale(sfm::vec2f scale) override;

    sfm::vec2i getMousePosition() const override;
    bool isPressed()              const override;
};


#endif // PHOTOSHOP_CANVAS_IMPLEMENTATION
