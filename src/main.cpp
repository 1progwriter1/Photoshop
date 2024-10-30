#include <controller/controller.hpp>
#include <systemdata.h>
#include <sys/my_exceptions.hpp>
#include <api_impl/windows.hpp>
#include <sys/plugins.hpp>


int main()
{
	try
	{
		IWindowContainer *root = getRootWindow();

		Model model;
		Controller ctrl( &model, root);

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


/*


IWindowContainer root_window = nullptr;

class WindowManager
{
    RenderWindow window_;
    IWindowContainer *container_;
public:
    some functions
};


class Model
{
    IWindowContainer *container_;

};


class Controller
{
    WindowManager *manager_;
	Model *model_;
public:
	void getRequests();
	void proceedModel();
	void proceedView();
};

*/


