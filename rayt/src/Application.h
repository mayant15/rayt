#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Application
{
public:
    inline void Run()
    {
        startup();
        tick();
        shutdown();
    }
private:
    GLFWwindow* pWindow = nullptr;
    VkInstance instance {};
    VkPhysicalDevice device = VK_NULL_HANDLE;

private:
    void startup();
    void tick();
    void shutdown();
};
