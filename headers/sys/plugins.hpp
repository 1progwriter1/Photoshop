#ifndef PLUGINS_FUNCTIONS
#define PLUGINS_FUNCTIONS


static const char *PLUGINS_PATH[] =
{
	"../build/libcanvas.dylib",
	"../build/libtoolbar.dylib",
	"../build/liboptionsbar.dylib",
	"../build/libmenubar.dylib",
	"../build/libps_button.dylib",
	"../build/libbrush.dylib",
	"../build/liberaser.dylib",
	"../build/librectangle.dylib",
	"../build/libellipse.dylib",
	"../build/libline.dylib",
	"../build/libmenu_items.dylib",
	// "../build/lib_blur_filter.dylib",
	// "../build/lib_negative_filter.dylib",
};



void actionPlugins( const char *func, bool is_load);


#endif // PLUGINS_FUNCTIONS
