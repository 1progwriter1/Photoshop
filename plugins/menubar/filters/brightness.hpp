#ifndef BRIGHTNESS_FILTER_PLUGIN
#define BRIGHTNESS_FILTER_PLUGIN


#include "../menu_items.hpp"
#include "api/api_canvas.hpp"
#include "../control_panel.hpp"
#include <set>


class BrightnessFilterControlPanel;


class BrightnessFilter : public TextButton
{
    std::unique_ptr<BrightnessFilterControlPanel> panel_;
    friend class BrightnessFilterAction;

    bool is_applied_ = false;
    bool is_ok_ = false;
    bool is_cancel_ = false;

    std::unique_ptr<IMementable<ILayerSnapshot>> memento_;
    std::unique_ptr<ILayerSnapshot> snapshot_;
public:
    BrightnessFilter(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void draw(IRenderWindow* renderWindow) override;

    void updateState(const IRenderWindow *renderWindow, const Event &event);
    void updatePanel(const IRenderWindow *renderWindow, const Event &event);

    void setOk(bool value);
    void setCancel(bool value);
    void setApplied(bool value);
    void unsetAll();
};


class BrightnessFilterAction : public AUndoableAction
{
    ICanvas *canvas_;
    BrightnessFilter *button_;

    ILayer *main_layer_ = nullptr;
    ILayer *tmp_layer_ = nullptr;

    void applyToTmpLayer();
    void applyToMainLayer();
    void removeTmpLayer();
public:
    BrightnessFilterAction(BrightnessFilter *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


class BrightnessFilterControlPanel : public ControlPanel
{
    friend class BrightnessFilterControlPanelAction;

    std::unique_ptr<sfm::RectangleShape> frame_ = std::make_unique<sfm::RectangleShape>();
    std::set<std::pair<int, int>> points_;
    std::vector<std::pair<int, int>> interpolated_;
    sfm::IntRect draw_rect_;

    std::unique_ptr<sfm::IImage> image_ = std::make_unique<sfm::Image>();
    std::unique_ptr<sfm::ITexture> texture_ = std::make_unique<sfm::Texture>();
    std::unique_ptr<sfm::ISprite> sprite_ = std::make_unique<sfm::Sprite>();

    BrightnessFilter *parent_;
    bool is_added_ = false;

    friend class ControlPanelAction;
public:
    BrightnessFilterControlPanel();

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void draw(IRenderWindow* renderWindow) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);

    void setSize(const vec2u& size) override;
    void setPos(const vec2i& pos) override;
    void setFilter(BrightnessFilter *parent);

    const std::vector<std::pair<int, int>> &getBrightnessPoints() const &;
private:
    void drawPoint(sfm::vec2i pos, sfm::Color color, int radius);
};


class BrightnessFilterControlPanelAction : public AAction
{
    BrightnessFilterControlPanel *button_;
public:
    BrightnessFilterControlPanelAction(BrightnessFilterControlPanel *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
private:
    void interpolate();
    sfm::vec2i interpolatePoint( sfm::vec2i p0, sfm::vec2i p1, sfm::vec2i p2, sfm::vec2i p3, float t);
};


#endif // BRIGHTNESS_FILTER_PLUGIN
