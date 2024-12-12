#ifndef PLUGIN_MENU_TOOLS
#define PLUGIN_MENU_TOOLS


#include "menu_items.hpp"


class Seagfault : public TextButton
{
    friend class SeagfaultAction;
public:
    Seagfault(wid_t init_id, std::unique_ptr<sfm::IFont> font, std::unique_ptr<sfm::IText> text,
                std::unique_ptr<sfm::IRectangleShape> init_shape);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    void updateState(const IRenderWindow *renderWindow, const Event &event);
};


class SeagfaultAction : public AAction
{
    Seagfault *button_;
public:
    SeagfaultAction(Seagfault *button, const IRenderWindow *renderWindow, const Event &event);

    bool execute(const Key &key) override;
    bool isUndoable(const Key &key) override;
};


#endif // PLUGIN_MENU_TOOLS
