#include <rayt/surface.h>

#include <stdexcept>

namespace rayt
{
    surface_t::surface_t(VkInstance* instance_ptr, void* window_ptr) : m_instance_ptr(instance_ptr)
    {
        if (glfwCreateWindowSurface(*instance_ptr, (GLFWwindow*) window_ptr, nullptr, &m_surface) != VK_SUCCESS)
        {
            throw std::runtime_error {"Failed to create window surface."};
        }
    }

    surface_t::~surface_t()
    {
        vkDestroySurfaceKHR(*m_instance_ptr, m_surface, nullptr);
    }
}
