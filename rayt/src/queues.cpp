#include <rayt/queues.h>

namespace rayt
{
    queue_family_indices_t::queue_family_indices_t(const physical_device_t& device)
    {
        auto vk_device = device.get_vk();

        unsigned int num_queue_families = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(vk_device, &num_queue_families, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(num_queue_families);
        vkGetPhysicalDeviceQueueFamilyProperties(vk_device, &num_queue_families, queue_families.data());

        for (unsigned int i = 0; i < num_queue_families; ++i)
        {
            const auto& family = queue_families[i];

            // Check if we have a graphics queue
            if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                m_graphics_family = i;
            }

            if (is_complete()) break;
        }
    }

//    queue_t::queue_t(const device_t& device, unsigned int queue_family_index, unsigned int queue_index)
//    {
//        VkQueue queue;
//        vkGetDeviceQueue(device.get_vk(), queue_family_index, queue_index, &queue);
//        m_queue = queue;
//    }
}
