#ifndef PHOTOSHOP_MODEL
#define PHOTOSHOP_MODEL


#include <list>
#include <model/layer.hpp>


class Model
{
    std::list<graphic::Layer> layers_;
    friend class Controller;
};


#endif // PHOTOSHOP_MODEL
