#ifndef PLUGINS_FUNCTIONS
#define PLUGINS_FUNCTIONS


static const char *PLUGINS_PATH[] =
{
	"../build/libcanvas.dylib",
	"../build/libtoolbar.dylib",
	"../build/liboptionsbar.dylib",
	"../build/libmenubar.dylib",
	"../build/libbrush.dylib",
	"../build/liberaser.dylib",
	"../build/librectangle.dylib",
	// "../build/libellipse.dylib",
	// "../build/libline.dylib",
	// "../build/libfilter.dylib",
	// "../build/libbrightness_filter.dylib",
	// "../build/libnegative_filter.dylib",
	// "../build/libbarelief_filter.dylib",
	// "../build/libgauss_bluer_filter.dylib",
	// "../build/libcontrast_filter.dylib",
};



void actionPlugins( const char *func, bool is_load);


#endif // PLUGINS_FUNCTIONS
