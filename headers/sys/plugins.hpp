#ifndef PLUGINS_FUNCTIONS
#define PLUGINS_FUNCTIONS


static const char *PLUGINS_PATH[] =
{
	"../build/libcanvas.dylib",
	"../build/libtoolbar.dylib",
	"../build/libbrush.dylib",
	"../build/liberaser.dylib",
	"../build/librectangle.dylib",
	"../build/libellipse.dylib",
};

//  g++ ../src/api_impl/api_system.cpp ../src/api_impl/bar.cpp ../src/api_impl/canvas.cpp ../src/api_impl/sfm.cpp ../src/api_impl/windows.cpp  -dynamiclib -o libapi_photoshop.dylib -I../api -I../headers/ -std=c++11 -I/opt/homebrew/include/ -L/opt/homebrew/lib/ -lsfml-graphics -lsfml-window -lsfml-system ../src/sys/my_exceptions.cpp -I../../MyLibraries/headers


void actionPlugins( const char *func);


#endif // PLUGINS_FUNCTIONS
