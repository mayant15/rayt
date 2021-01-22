#include <rayt/logger.h>

#include "application.h"

#include <exception>

int main(int argc, char** argv)
{
    application_t app;
	try
	{
		app.tick();
	}
	catch (const std::exception& e)
	{
		LOG_ERROR(e.what());
		return EXIT_FAILURE;
	}

	// app will be destroyed here, cleanup will be performed

	return EXIT_SUCCESS;
}
