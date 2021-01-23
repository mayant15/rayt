#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "rayt_export.h"

namespace rayt
{
    /** @brief Client interface for window operations */
    class RAYT_EXPORT window_t
    {
        GLFWwindow* m_native_ptr = nullptr;

    public:
        window_t();
        ~window_t() noexcept;

        /** @brief Poll for native window events */
        void poll_events();

        /** @brief Query if window should close this frame */
        [[nodiscard]] bool should_close() const;
    };
}
