#include <cstdio>

#include <controller/controller.hpp>
#include <systemdata.h>
#include <sys/my_exceptions.hpp>
#include <plugins/plugins.hpp>


int main()
{
	try
	{
		Model model;
		WindowManager view( 1400, 800);
		Controller ctrl( &model, &view);

		runPlugin();

		while ( ctrl.inProgress() )
		{
			ctrl.getRequests();
			ctrl.proceedModel();
			ctrl.proceedView();
		}
	} catch ( my_std::exception *exc )
	{
		exc->dumpInfo();
	}

	return SUCCESS;
}


