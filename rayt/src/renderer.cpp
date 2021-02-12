#include <rayt/renderer.h>

namespace rayt
{
    /**
     * @brief Check if the physical device is appropriate for our purposes
     * @param device Physical device to consider
     * @return true if it has all the required queues
     */
    static bool is_suitable_device(const physical_device_t& device)
    {
        queue_family_indices_t indices { device };
        return indices.is_complete();
    }

    renderer_t::renderer_t(window_t* p_window)
    {
        // Choose physical device
        auto devices = enumerate_physical_devices(m_instance);
        for (const auto& device : devices)
        {
            if (is_suitable_device(device) && device.is_valid())
            {
                m_physical_device = device;
                break;
            }
        }

        // Create logical device
        m_device = device_t { m_physical_device };

        // Find the graphics queue
//        auto indices = queue_family_indices_t { m_physical_device };
//        m_queue = queue_t { m_device, indices.m_graphics_family.value(), 0};

        if (p_window != nullptr)
        {
            m_window_ptr = p_window;
        }
    }
}
