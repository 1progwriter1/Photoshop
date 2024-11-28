#include <api_impl/actions.hpp>
#include <cassert>


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

    return false;
}
