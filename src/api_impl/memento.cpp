#include <api_impl/memento.hpp>
#include <cassert>


template<typename SnapShot>
std::unique_ptr<SnapShot> AMementable<SnapShot>::save()
{
    assert( 0 && "Not implemented" );
    return nullptr;
}


template<typename SnapShot>
void AMementable<SnapShot>::restore(SnapShot* snapshot)
{
    assert( 0 && "Not implemented" );
}
