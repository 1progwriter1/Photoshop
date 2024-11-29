#include <controller/controller.hpp>
#include <systemdata.h>
#include <sys/my_exceptions.hpp>
#include <api_impl/windows.hpp>
#include <sys/plugins.hpp>


int main()
{
	try
	{
		Controller ctrl(psapi::getRootWindow());

		actionPlugins("loadPlugin", true);

		while ( ctrl.inProgress() )
		{
			ctrl.getRequests();
			ctrl.proceedView();
		}

		actionPlugins("unloadPlugin", false);

	} catch ( my_std::exception &exc )
	{
		exc.dumpInfo();
	}

	return SUCCESS;
}


