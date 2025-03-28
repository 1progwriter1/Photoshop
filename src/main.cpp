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

		actionPlugins("onLoadPlugin", true);

		sf::Image icon;
		if (!icon.loadFromFile( "../assets/images/icon.png"))
		{
			throw MY_EXCEPTION( "Failed to load icon", nullptr);
		}

		ctrl.getSFMLWindow().setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		while (ctrl.inProgress())
		{
			ctrl.getRequests();
			ctrl.proceedView();
		}

		actionPlugins("onUnloadPlugin", false);

	} catch (my_std::exception &exc)
	{
		exc.dumpInfo();
	}

	return SUCCESS;
}
