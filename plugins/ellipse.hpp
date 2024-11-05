#ifndef ELLIPSE_PLUGIN
#define ELLIPSE_PLUGIN


#include "../headers/api_impl/bar.hpp"


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


class Ellipse : public ABarButton
{
};


#endif // ELLIPSE_PLUGIN
