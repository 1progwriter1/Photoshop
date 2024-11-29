#ifndef PHOTOSHOP_ACTIONS_IMPLEMENTATION
#define PHOTOSHOP_ACTIONS_IMPLEMENTATION


#include <api_actions.hpp>
#include <api_photoshop.hpp>
#include <deque>
#include <memory>


using psapi::IAction;
using psapi::IUndoableAction;
using psapi::sfm::IRenderWindow;
using psapi::sfm::Event;

class AAction : public psapi::IAction
{
protected:
    const IRenderWindow *render_window_;
    const Event &event_;
public:
    AAction(const IRenderWindow *render_window, const Event &event);

    virtual bool execute(const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
};


class AUndoableAction : public psapi::IUndoableAction
{
public:
    virtual bool undo(const Key& key) override;
    virtual bool redo(const Key& key) override;
};


class ActionsController : public psapi::AActionController
{
    std::deque<std::unique_ptr<IUndoableAction>> done_actions_;
    std::deque<std::unique_ptr<IUndoableAction>> undone_actions_;
public:
    bool execute(std::unique_ptr<IAction> action) override;
    bool undo() override;
    bool redo() override;
};


#endif // PHOTOSHOP_ACTIONS_IMPLEMENTATION
