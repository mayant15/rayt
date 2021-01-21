#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


struct ApplicationInfo
{
    unsigned int WindowHeight;
    unsigned int WindowWidth;
};

class Application
{
public:
    Application(const ApplicationInfo& info) : _options(info), _window(nullptr) {}

    inline void Run()
    {
        startup();
        tick();
        shutdown();
    }

private:
    GLFWwindow* _window;
    ApplicationInfo _options;

    VkInstance _instance{};

private:
    void startup();
    void tick();
    void shutdown();

    void createVkInstance();
};
