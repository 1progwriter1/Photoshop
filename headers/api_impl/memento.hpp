#ifndef PHOTOSHOP_MEMENTO_IMPLEMENTATION
#define PHOTOSHOP_MEMENTO_IMPLEMENTATION


#include <api_memento.hpp>


class ALayerSnapshot : public psapi::ILayerSnapshot
{
public:
};


class ACanvasSnapshot : public psapi::ICanvasSnapshot
{
public:
};


template<typename SnapShot>
class AMementable : public psapi::IMementable<SnapShot>
{
public:
    virtual std::unique_ptr<SnapShot> save() override;
    virtual void restore(SnapShot* snapshot) override;
};


#endif // PHOTOSHOP_MEMENTO_IMPLEMENTATION
