#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include "rayt_export.h"

namespace rayt
{
    class RAYT_EXPORT window_t
    {
        GLFWwindow* m_native_ptr = nullptr;
    public:
        window_t();

        ~window_t() noexcept;

        void poll_events();

        [[nodiscard]] bool should_close() const;

        [[nodiscard]] void* get_native_ptr() const;
    };
}
