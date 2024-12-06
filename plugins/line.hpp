#ifndef LINE_PLUGIN
#define LINE_PLUGIN


#include "../headers/api_impl/bar.hpp"
#include "../api/api_canvas.hpp"
#include "../headers/api_impl/canvas.hpp"


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


class Line : public ABarButton
{
    sfm::RectangleShape shape_;
    sfm::vec2i begin_pos_ = sfm::vec2i();
    sfm::vec2i last_mouse_pos_ = sfm::vec2i();

    bool draw_ = false;

    ICanvas *canvas_;
    ILayer *layer_ = nullptr;

    friend class LineAction;
public:
    Line( wid_t init_id, std::unique_ptr<sfm::Texture> &init_texture, std::unique_ptr<sfm::Sprite> &init_sprite);

    void draw( sfm::IRenderWindow *renderWindow) override;
    std::unique_ptr<IAction> createAction(const IRenderWindow *renderWindow, const Event &event) override;

private:
    void drawLine( const sfm::IRenderWindow *renderWindow, ILayer *layer, bool is_temp_layer);
    void setAngleAndLength( sfm::vec2i begin_pos, sfm::vec2i end_pos);
    bool isOnCanvas( sfm::vec2i mouse_pos);

    void updateState(const IRenderWindow *renderWindow, const Event &event);
};



class LineAction : public AUndoableAction
{
    Line *line_;
public:
    LineAction(Line *line, const IRenderWindow *renderWindow, const Event &event);

    bool isUndoable(const Key &key) override;
    bool execute(const Key &key) override;

    bool undo(const Key &key) override;
    bool redo(const Key &key) override;
};


#endif // LINE_PLUGIN
