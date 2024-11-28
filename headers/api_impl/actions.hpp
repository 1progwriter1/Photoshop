#ifndef PHOTOSHOP_ACTIONS_IMPLEMENTATION
#define PHOTOSHOP_ACTIONS_IMPLEMENTATION


#include <api_actions.hpp>


using psapi::IAction;

class AAction : public psapi::IAction
{
public:
    bool execute(const Key& key) override;
    bool isUndoable(const Key& key) override;
};


class AUndoableAction : public psapi::IUndoableAction
{
public:
    bool undo(const Key& key) override;
    bool redo(const Key& key) override;
};


#endif // PHOTOSHOP_ACTIONS_IMPLEMENTATION
