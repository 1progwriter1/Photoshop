#ifndef PLUGINS_FUNCTIONS
#define PLUGINS_FUNCTIONS


const char *PLUGINS_PATH[] =
{
	"../plugins/libtest_plugin.dylib"
};

// g++ ../src/api_impl/api_sfm.cpp ../src/api_impl/bar.cpp ../src/api_impl/canvas.cpp ../src/api_impl/sfm.cpp ../src/api_impl/windows.cpp -dynamiclib -o libapi_photoshop.dylib -I../api -I../headers/ -std=c++11 -I/opt/homebrew/include/


void actionPlugins( const char *func);


#endif // PLUGINS_FUNCTIONS
