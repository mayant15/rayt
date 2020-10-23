#include "Application.h"


void Application::startup()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    _window =  glfwCreateWindow(_options.WindowWidth, _options.WindowHeight, "rayt", nullptr, nullptr);
}

void Application::tick()
{
    while(!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
    }
}

void Application::shutdown()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}
