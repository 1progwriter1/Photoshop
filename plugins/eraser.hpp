#ifndef ERASER_PLUGIN
#define ERASER_PLUGIN


#include "../headers/api_impl/bar.hpp"


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


class Eraser : public ABarButton
{
};


#endif // ERASER_PLUGIN
