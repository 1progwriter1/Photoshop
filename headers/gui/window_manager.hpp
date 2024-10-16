#ifndef WINDOW_MANAGER
#define WINDOW_MANAGER


#include <SFML/Graphics.hpp>


class WindowManager
{

    sf::RenderWindow screen_;
    unsigned int width_;
    unsigned int height_;

    friend class Controller;

    // windows
public:
    WindowManager( unsigned int init_width, unsigned int init_height, const char *title = "hello");
    ~WindowManager() = default;
};


#endif // WINDOW_MANAGER
