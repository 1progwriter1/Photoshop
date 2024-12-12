#include <api_impl/actions.hpp>
#include <cassert>
#include <iostream>


AAction::AAction(const IRenderWindow *render_window, const Event &event)
    :   render_window_(render_window), event_(event) {}


bool AAction::isUndoable(const Key& key)
{
    assert( 0 && "Not implemented" );

    return false;
}


bool AAction::execute(const Key& key)
{
    assert( 0 && "Not implemented" );

    return false;
}


AUndoableAction::AUndoableAction(const IRenderWindow *render_window, const Event &event)
    :   render_window_(render_window), event_(event) {}


bool AUndoableAction::undo(const Key& key)
{
    assert( 0 && "Not implemented" );

    return false;
}


bool AUndoableAction::redo(const Key& key)
{
    assert( 0 && "Not implemented" );
}


bool ActionController::execute(std::unique_ptr<IAction> action)
{
    bool res = actionExecute(action.get());
    if ( isUndoableAction(action.get()) )
    {
        psapi::IUndoableAction *undoable_action = dynamic_cast<psapi::IUndoableAction *>(action.release());
        assert( undoable_action );

        done_actions_.emplace_back( std::unique_ptr<IUndoableAction>(undoable_action));
        if ( done_actions_.size() > 10 )
            done_actions_.pop_front();
    }
    return res;
}


bool ActionController::undo()
{
    if ( done_actions_.empty() )
        return true;

    std::unique_ptr<psapi::IUndoableAction> action = std::move(done_actions_.back());
    done_actions_.pop_back();
    bool res = actionUndo(action.get());

    undone_actions_.emplace_back(std::move(action));
    if ( undone_actions_.size() > 10 )
        undone_actions_.pop_front();

    return res;
}


bool ActionController::redo()
{
    if ( undone_actions_.empty() )
        return true;

    std::unique_ptr<psapi::IUndoableAction> action = std::move(undone_actions_.back());
    undone_actions_.pop_back();
    bool res = actionRedo(action.get());

    done_actions_.emplace_back(std::move(action));
    if ( done_actions_.size() > 10 )
        done_actions_.pop_front();

    return res;
}


psapi::AActionController *ActionController::getInstance()
{
    static ActionController instance;
    return &instance;
}


psapi::AActionController *psapi::getActionController()
{
    return ActionController::getInstance();
}
