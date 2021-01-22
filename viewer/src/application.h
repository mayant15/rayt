#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class application_t
{
public:
    application_t();
    ~application_t();

    void tick();

private:
    GLFWwindow* p_window = nullptr;
    VkInstance instance{};
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphics_queue{};
};
