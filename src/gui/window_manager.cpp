#include <gui/window_manager.hpp>


WindowManager::WindowManager( unsigned int init_width, unsigned int init_height, const char *title /*= "hello"*/)
    :   screen_( sf::VideoMode( init_width, init_height), title),
        width_( init_width), height_( init_height) {}
