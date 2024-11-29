#ifndef PHOTOSHOP_ACTIONS_IMPLEMENTATION
#define PHOTOSHOP_ACTIONS_IMPLEMENTATION


#include <api_actions.hpp>
#include <deque>


using psapi::IAction;


class AAction : public psapi::IAction
{
public:
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
    std::deque<std::unique_ptr<psapi::IUndoableAction>> done_actions_;
    std::deque<std::unique_ptr<psapi::IUndoableAction>> undone_actions_;
public:
    bool execute(std::unique_ptr<IAction> action) override;
    bool undo() override;
    bool redo() override;
};


#endif // PHOTOSHOP_ACTIONS_IMPLEMENTATION
