#ifndef PHOTOSHOP_CANVAS_IMPLEMENTATION
#define PHOTOSHOP_CANVAS_IMPLEMENTATION


#include <api_canvas.hpp>
#include <api_impl/windows.hpp>


using namespace psapi;

using sfm::Color;


class Layer : public ILayer
{
    vec2i pos_;
    vec2u size_;

    std::vector<Color> pixels_;

    friend class Canvas;
public:
    Layer( vec2u size, vec2i pos = vec2i());
    ~Layer() = default;

    Color getPixel(sfm::vec2i pos) const override;
    void setPixel(sfm::vec2i pos, sfm::Color pixel) override;
};


class Canvas : public ICanvas
{
    std::list<std::unique_ptr<ILayer>> layers_;

    bool is_active_;
    const IWindow *parent_;
    size_t active_layer_index_;

    vec2u size_;
    vec2i pos_;

    std::unique_ptr<sfm::ITexture> texture_;
    std::unique_ptr<sfm::ISprite> sprite_;
public:
    Canvas( vec2i init_pos, vec2u init_size);
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

    bool isWindowContainer() const override;
    wid_t getId() const override;
    vec2i getPos() const override;
    vec2u getSize() const override;
    void setParent(const IWindow* parent) override;
    bool isActive() const override;
    void forceActivate() override;
    void forceDeactivate() override;
    IWindow *getWindowById( wid_t id) override;
    const IWindow *getWindowById( wid_t id) const override;

    void draw( IRenderWindow *removeWindow) override;
    bool update( const IRenderWindow *renderWindow, const Event &event) override;
};


#endif // PHOTOSHOP_CANVAS_IMPLEMENTATION
