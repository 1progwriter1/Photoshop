#include <api_impl/actions.hpp>
#include <cassert>


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


bool AUndoableAction::undo(const Key& key)
{
    assert( 0 && "Not implemented" );

    return false;
}


bool AUndoableAction::redo(const Key& key)
{
    assert( 0 && "Not implemented" );
}


bool ActionsController::execute(std::unique_ptr<IAction> action)
{
    std::unique_ptr<int> n = std::make_unique<int>(10);
    bool res = actionExecute(action.get());
    if ( isUndoableAction(action.get()) )
    {
        psapi::IUndoableAction *undoable_action = dynamic_cast<psapi::IUndoableAction *>(action.get());
        assert( undoable_action );

        done_actions_.emplace_back( std::unique_ptr<psapi::IUndoableAction>( undoable_action));
        if ( done_actions_.size() > 10 )
            done_actions_.pop_front();
    }
    return res;
}


bool ActionsController::undo()
{
    std::unique_ptr<psapi::IUndoableAction> action = std::move(done_actions_.back());
    bool res = actionUndo(done_actions_.back().get());
    done_actions_.pop_back();

    undone_actions_.emplace_back(std::move(action));
    if ( undone_actions_.size() > 10 )
        undone_actions_.pop_front();

    return res;
}


bool ActionsController::redo()
{
    std::unique_ptr<psapi::IUndoableAction> action = std::move(undone_actions_.back());
    bool res = actionRedo(undone_actions_.back().get());
    undone_actions_.pop_back();

    done_actions_.emplace_back(std::move(action));
    if ( done_actions_.size() > 10 )
        done_actions_.pop_front();

    return res;
}
