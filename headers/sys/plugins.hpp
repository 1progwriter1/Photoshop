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
	"../build/libline.dylib",
};



void actionPlugins( const char *func, bool is_load);


#endif // PLUGINS_FUNCTIONS
