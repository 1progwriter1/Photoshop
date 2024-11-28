#ifndef PHOTOSHOP_CANVAS_IMPLEMENTATION
#define PHOTOSHOP_CANVAS_IMPLEMENTATION


#include <api_canvas.hpp>
#include <api_impl/windows.hpp>
#include <api_impl/scroll.hpp>


using namespace psapi;

using sfm::Color;

const sfm::vec2u CANVAS_SECTOR_SIZE = vec2u( 1480, 790);
const sfm::vec2i CANVAS_SECTOR_POS = vec2i( 120, 110);


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

    drawable_id_t addDrawable(const sfm::Drawable *object) override;
    void removeDrawable(drawable_id_t id) override;
    void removeAllDrawables() override;

    vec2u getSize() const override;
};


class Canvas : public ICanvas, public VerticalScroll, public HorizontalScroll
{
    std::list<std::unique_ptr<ILayer>> layers_;
    std::unique_ptr<ILayer> temp_layer_;

    bool is_active_;
    const IWindow *parent_;
    size_t active_layer_index_;

    vec2u size_;
    vec2i pos_;
    vec2f scale_ = vec2f( 1, 1);
    vec2i mouse_pos_ = vec2i( 0, 0);

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

    void setPos (sfm::vec2i pos)  override;
    void setSize(sfm::vec2u size) override;
    void setZoom(sfm::vec2f zoom) override;

    sfm::vec2i getMousePosition() const override;

    virtual bool isPressedRightMouseButton() const override;
    virtual bool isPressedLeftMouseButton()  const override;
    virtual bool isPressedScrollButton()     const override;

    wid_t getId() const override;
    vec2i getPos() const override;
    vec2u getSize() const override;

    void setParent(const IWindow* parent) override;

    bool isWindowContainer() const override;
    bool isActive() const override;

    void forceActivate() override;
    void forceDeactivate() override;

    IWindow *getWindowById( wid_t id) override;
    const IWindow *getWindowById( wid_t id) const override;

    void draw(IRenderWindow *removeWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
};


#endif // PHOTOSHOP_CANVAS_IMPLEMENTATION
