#include "Application.h"
#include "Debug.h"
#include "Config.h"

#include <exception>

int main(int argc, char** argv)
{
    Application app;
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
