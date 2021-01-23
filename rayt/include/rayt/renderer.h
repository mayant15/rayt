#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include "rayt_export.h"

#include <vector>

namespace rayt
{
    class RAYT_EXPORT renderer_t
    {
        VkInstance m_instance {};
        VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
        VkDevice m_device = VK_NULL_HANDLE;
        VkQueue m_graphics_queue {};

    public:
        renderer_t();

        ~renderer_t() noexcept;
    };
}
