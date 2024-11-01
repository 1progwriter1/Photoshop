#include <sys/plugins.hpp>
#include <dlfcn.h>
#include <sys/my_exceptions.hpp>


void actionPlugins( const char *func)
{
	for ( const auto &plugin_path : PLUGINS_PATH )
	{
		void *handle = dlopen( plugin_path, RTLD_LAZY);
		if ( !handle )
		{
			fprintf( stderr, "Failed to load plugin [%s]: %s\n", plugin_path, dlerror());
			throw MY_EXCEPTION( "plugin load error", nullptr);
		}

		bool (*pluginFunction)() = (bool (*)()) dlsym(handle, func);
		if ( char *error = dlerror() )
		{
			fprintf( stderr, "Failed to find function [%s]: %s\n", func, dlerror());
			dlclose(handle);
			throw MY_EXCEPTION( "plugin load error", nullptr);
		}

		pluginFunction();
    	dlclose(handle);
	}
}
