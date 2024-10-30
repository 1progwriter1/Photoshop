#include <stdio.h>
#include "../api/api_photoshop.hpp"
#include <dlfcn.h>

#define DYNAMIC_LIB_PSAPI "libapi_photoshop.dylib"

extern "C"
{


bool loadPlugin();
void unloadPlugin();


static void *handle = NULL;


bool loadPlugin()
{

    handle = dlopen( DYNAMIC_LIB_PSAPI, RTLD_LAZY);
    if ( !handle )
    {
        fprintf( stderr, "Failed to load dylib [%s]\n", DYNAMIC_LIB_PSAPI);
        return false;
    }
    psapi::IWindowContainer *(*getRootWindow)() = (psapi::IWindowContainer *(*)()) dlsym(handle, "getRootWindow");
    if ( !getRootWindow )
    {
        fprintf( stderr, "Failed to load function [getRootWindow]\n");
        unloadPlugin();
        return false;
    } else
    {
        printf( "getRootWindow loaded\n");
    }

    return true;
}


void unloadPlugin()
{
    if ( handle )
    {
        dlclose( handle);
    }
}


}
