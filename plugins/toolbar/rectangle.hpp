#ifndef RECTANGLE_PLUGIN
#define RECTANGLE_PLUGIN


#include "api_impl/bar/button.hpp"
#include "api/api_canvas.hpp"
#include "api/api_canvas.hpp"
#include "api/api_sfm.hpp"
#include "api_impl/memento.hpp"


using namespace psapi;


extern "C"
{
    bool onLoadPlugin();
    void onUnloadPlugin();
}


class Rectangle : public ABarButton
{
    vec2i left_upper_edge_ = {};
    vec2i last_mouse_pos_ = {};
    sfm::Color color_ = sfm::Color::getStandardColor(psapi::sfm::Color::Type::Cyan);
    sfm::IntRect canvas_rect_ = {};

    bool draw_ = false;

    IColorPalette *palette_ = nullptr;
    ICanvas *canvas_;
    ILayer *layer_ = nullptr;
    IOptionsBar *options_bar_;

    bool options_added_ = false;
    std::vector<std::unique_ptr<IWindow>> options_;
    std::vector<wid_t> id_;

    friend class RectangleAction;
    friend class UndoableRectangleAction;
public:
    Rectangle( wid_t init_id, std::unique_ptr<sfm::ITexture> &init_texture, std::unique_ptr<sfm::ISprite> &init_sprite);

    void draw( sfm::IRenderWindow *renderWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;

    void drawRectangle( const sfm::IRenderWindow *renderWindow, ILayer *layer, bool is_temp_layer);
    bool isOnCanvas( sfm::vec2i mouse_pos);

    void setColor( const sfm::Color &new_color);
    const sfm::Color &getColor() const;

    void addOptions();
    void removeOptions();
    void createOptions();

    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class RectangleAction : public AUndoableAction
{
    Rectangle *rectangle_;
public:
    RectangleAction(Rectangle *rectangle, const IRenderWindow *renderWindow, const Event &event);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


class UndoableRectangleAction : public AUndoableAction
{
    Rectangle *rectangle_;
    std::unique_ptr<ILayerSnapshot> snapshot_;
    std::unique_ptr<IMementable<ILayerSnapshot>> memento_;
public:
    UndoableRectangleAction(Rectangle *rectangle, const IRenderWindow *renderWindow, const Event &event);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


#endif // RECTANGLE_PLUGIN
