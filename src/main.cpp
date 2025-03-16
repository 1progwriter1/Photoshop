#include <controller/controller.hpp>
#include <systemdata.h>
#include <sys/my_exceptions.hpp>
#include <api_impl/windows.hpp>
#include <sys/plugins.hpp>


// TODO: текст рисуется относительно lowerBound, а не относительно getLocalBounds.
// TODO: исправить UNSharpMask
// TODO: spline
// TODO: Изменить интерфейс, так, чтобы он стал похож на Final Cut Pro or Adobe Photoshop
// TODO: изменяемый в runtime интерфейс


// * текст рисуется относительно lowerBound, а не относительно getLocalBounds.
// * исправить UNSharpMask
// * spline
// * Изменить интерфейс, так, чтобы он стал похож на Final Cut Pro or Adobe Photoshop
// TODO: изменяемый в runtime интерфейс


int main()
{
	try
	{
		Controller ctrl(psapi::getRootWindow());

		actionPlugins("onLoadPlugin", true);

		sf::Image icon;

		if ( !icon.loadFromFile( "../assets/images/icon.png") )
			throw MY_EXCEPTION( "Failed to load icon", nullptr);

		ctrl.getSFMLWindow().setIcon( icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		while ( ctrl.inProgress() )
		{
			ctrl.getRequests();
			ctrl.proceedView();
		}

		actionPlugins("onUnloadPlugin", false);

	} catch ( my_std::exception &exc )
	{
		exc.dumpInfo();
	}

	return SUCCESS;
}


