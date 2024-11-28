#ifndef PHOTOSHOP_ACTIONS_IMPLEMENTATION
#define PHOTOSHOP_ACTIONS_IMPLEMENTATION


#include <api_actions.hpp>


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


#endif // PHOTOSHOP_ACTIONS_IMPLEMENTATION
