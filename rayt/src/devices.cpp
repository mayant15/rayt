#include <rayt/devices.h>
#include <rayt/config.h>
#include <rayt/queues.h>

#include <stdexcept>

namespace rayt
{
    bool physical_device_t::is_valid() const
    {
        return m_physical_device != VK_NULL_HANDLE;
    }

    std::vector<physical_device_t> enumerate_physical_devices(instance_t& instance)
    {
        // Find a suitable device
        unsigned int num_devices = 0;
        vkEnumeratePhysicalDevices(instance.get_vk(), &num_devices, nullptr);

        if (num_devices == 0)
        {
            throw std::runtime_error {"Available GPUs do not support Vulkan"};
        }

        std::vector<VkPhysicalDevice> available_devices (num_devices);
        vkEnumeratePhysicalDevices(instance.get_vk(), &num_devices, available_devices.data());

        // Create physical device classes
        std::vector<physical_device_t> devices;
        for (auto vk_device : available_devices)
        {
            devices.emplace_back(vk_device);
        }

        return devices;
    }

    device_t::device_t(const physical_device_t& physical_device)
    {
#ifdef RAYT_ENABLE_VALIDATION_LAYERS
        m_validation_layers = { RAYT_REQUIRED_VALIDATION_LAYERS };
#endif
        queue_family_indices_t indices {physical_device};

        float queue_priority = 1.0f;
        VkDeviceQueueCreateInfo queue_create_info {};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = indices.m_graphics_family.value();
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;

        // Use default device features
        VkPhysicalDeviceFeatures device_features {};

        VkDeviceCreateInfo device_create_info {};
        device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_create_info.pQueueCreateInfos = &queue_create_info;
        device_create_info.queueCreateInfoCount = 1;
        device_create_info.pEnabledFeatures = &device_features;

#ifdef RAYT_ENABLE_VALIDATION_LAYERS
        device_create_info.enabledLayerCount = static_cast<uint32_t> (m_validation_layers.size());
        device_create_info.ppEnabledLayerNames = m_validation_layers.data();
#else
        device_create_info.enabledLayerCount = 0;
#endif
        device_create_info.enabledExtensionCount = 0;

        if (vkCreateDevice(physical_device.get_vk(), &device_create_info, nullptr, &m_device) != VK_SUCCESS)
        {
            throw std::runtime_error {"Failed to create a logical device"};
        }
    }

    device_t::~device_t()
    {
        vkDestroyDevice(m_device, nullptr);
    }
}
