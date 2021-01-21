#include "Application.h"
#include "Debug.h"
#include "Config.h"

#include <exception>

int main(int argc, char** argv)
{
	ApplicationInfo info;
	info.WindowHeight = RAYT_WINDOW_HEIGHT;
	info.WindowWidth = RAYT_WINDOW_WIDTH;

    Application app (info);
	try
	{
		app.Run();
	}
	catch (const std::exception& e)
	{
		LOG_ERROR(e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
