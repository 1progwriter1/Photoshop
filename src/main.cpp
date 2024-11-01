#include <controller/controller.hpp>
#include <systemdata.h>
#include <sys/my_exceptions.hpp>
#include <api_impl/windows.hpp>
#include <sys/plugins.hpp>


int main()
{
	try
	{
		Model model;
		Controller ctrl( &model, psapi::getRootWindow());

		actionPlugins( "loadPlugin");

		while ( ctrl.inProgress() )
		{
			ctrl.getRequests();
			ctrl.proceedModel();
			ctrl.proceedView();
		}

		actionPlugins( "unloadPlugin");

	} catch ( my_std::exception &exc )
	{
		exc.dumpInfo();
	}

	return SUCCESS;
}


