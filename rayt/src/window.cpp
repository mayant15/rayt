#include <rayt/window.h>
#include <rayt/config.h>
#include <rayt/logger.h>

#include <exception>

namespace rayt
{

    window_t::window_t()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        m_native_ptr = glfwCreateWindow(RAYT_WINDOW_WIDTH, RAYT_WINDOW_HEIGHT, RAYT_WINDOW_TITLE, nullptr, nullptr);
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
}
