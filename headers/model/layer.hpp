#ifndef GRAPHIC_LAYER
#define GRAPHIC_LAYER


#include <SFML/Graphics.hpp>


namespace graphic
{


class Layer
{
    sf::Sprite *sprite_;

    // lines
    // squares
    // circles
    // shapes ?
public:
    Layer() = default;
    ~Layer() = default;
};


} // graphic


#endif // GRAPHIC_LAYER
