#include "Application.h"

#include <iostream>

int main(int argc, char** argv)
{
	ApplicationInfo info;
	info.WindowHeight = 720;
	info.WindowWidth = 1024;

    Application app (info);
	try
	{
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}