#include <sys/plugins.hpp>
#include <dlfcn.h>
#include <sys/my_exceptions.hpp>


void actionPlugins( const char *func, bool is_load)
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

		if ( is_load && !pluginFunction() )
		{
			throw MY_EXCEPTION( "loadPlugin returned false", nullptr);
		}
	}
}
