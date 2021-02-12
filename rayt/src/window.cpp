#include <rayt/window.h>
#include <rayt/config.h>
#include <rayt/logger.h>

#include <exception>

namespace rayt
{

    window_t::window_t()
            : width(RAYT_WINDOW_WIDTH),
              height(RAYT_WINDOW_HEIGHT)
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        m_native_ptr = glfwCreateWindow(width, height, RAYT_WINDOW_TITLE, nullptr, nullptr);
    }


    window_t::~window_t() noexcept
    {
        try
        {
            glfwDestroyWindow(m_native_ptr);
            glfwTerminate();
        }
        catch (const std::exception& e)
        {
            LOG_ERROR(e.what());
        }
    }


    void window_t::poll_events() // NOLINT(readability-convert-member-functions-to-static)
    {
        glfwPollEvents();
    }


    bool window_t::should_close() const
    {
        return glfwWindowShouldClose(m_native_ptr);
    }

    void window_t::create_surface(const VkInstance& instance, VkSurfaceKHR& surface)
    {
        if (glfwCreateWindowSurface(instance, m_native_ptr, nullptr, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error { "Failed to create window surface." };
        }
    }
}
