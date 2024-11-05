#ifndef RECTANGLE_PLUGIN
#define RECTANGLE_PLUGIN


#include "../headers/api_impl/bar.hpp"


extern "C"
{
    bool loadPlugin();
    void unloadPlugin();
}


class Rectangle : public ABarButton
{
};



#endif // RECTANGLE_PLUGIN
