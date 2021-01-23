#include <rayt/renderer.h>
#include <rayt/config.h>
#include <rayt/logger.h>
#include "queues.h"

#include <exception>

#define RAYT_VERSION \
    VK_MAKE_VERSION(RAYT_VERSION_MAJOR, RAYT_VERSION_MINOR, RAYT_VERSION_PATCH)

namespace rayt
{
    static const std::vector<const char*> LAYER_NAMES = {RAYT_REQUIRED_VALIDATION_LAYERS}; // NOLINT(cert-err58-cpp)

    // Instance utils
    static bool check_required_layer_support();

    static void create_instance(VkInstance& instance);

    // Device utils
    static queue_family_indices_t find_queue_families(const VkPhysicalDevice& device);

    static bool is_suitable_device(const VkPhysicalDevice& device);

    static void select_physical_device(VkInstance& instance, VkPhysicalDevice& device);

    static void create_logical_device(VkPhysicalDevice& physical_device, VkDevice& device, VkQueue& queue);

    renderer_t::renderer_t()
    {
        create_instance(m_instance);
        select_physical_device(m_instance, m_physical_device);
        create_logical_device(m_physical_device, m_device, m_graphics_queue);
    }

    renderer_t::~renderer_t() noexcept
    {
        try
        {
            vkDestroyDevice(m_device, nullptr);
            vkDestroyInstance(m_instance, nullptr);
        }
        catch (const std::exception& e)
        {
            LOG_ERROR(e.what());
        }
    }

    bool check_required_layer_support()
    {
        unsigned int num_layers;
        vkEnumerateInstanceLayerProperties(&num_layers, nullptr);

        std::vector<VkLayerProperties> available_layers(num_layers);
        vkEnumerateInstanceLayerProperties(&num_layers, available_layers.data());

        // Check if the required layers are available
        for (const auto* requested_layer : LAYER_NAMES)
        {
            bool is_present = false;
            for (const auto& layer : available_layers)
            {
                if (strcmp(requested_layer, layer.layerName) == 0)
                {
                    is_present = true;
                }
            }

            if (!is_present)
            {
                LOG_ERROR("Required layer not unavailable: {}", requested_layer);
                return false;
            }
        }

        return true;
    }

    void create_instance(VkInstance& instance)
    {
#ifdef RAYT_ENABLE_VALIDATION_LAYERS
        if (!check_required_layer_support())
        {
            throw std::runtime_error {"Requested validation layers are unavailable"};
        }
#endif

        // Setup application information
        VkApplicationInfo application_info {};
        application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        application_info.pEngineName = "No Engine";
        application_info.pApplicationName = "rayt";
        application_info.applicationVersion = RAYT_VERSION;
        application_info.engineVersion = RAYT_VERSION;
        application_info.apiVersion = VK_API_VERSION_1_2;

        // Setup instance information
        VkInstanceCreateInfo instance_create_info {};
        instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_create_info.pApplicationInfo = &application_info;

#ifdef RAYT_ENABLE_VALIDATION_LAYERS
        instance_create_info.enabledLayerCount = static_cast<uint32_t> (LAYER_NAMES.size());
        instance_create_info.ppEnabledLayerNames = LAYER_NAMES.data();
#else
        createInfo.enabledLayerCount = 0;
#endif

        // Get required extensions
        unsigned int num_extensions;
        const char** extensions = glfwGetRequiredInstanceExtensions(&num_extensions);
        instance_create_info.enabledExtensionCount = num_extensions;
        instance_create_info.ppEnabledExtensionNames = extensions;

        // Create instance
        if (vkCreateInstance(&instance_create_info, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create Vulkan instance");
        }
    }

    queue_family_indices_t find_queue_families(const VkPhysicalDevice& device)
    {
        queue_family_indices_t indices;

        unsigned int num_queue_families = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &num_queue_families, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(num_queue_families);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &num_queue_families, queue_families.data());

        for (unsigned int i = 0; i < num_queue_families; ++i)
        {
            const auto& family = queue_families[i];

            // Check if we have a graphics queue
            if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.m_graphics_family = i;
            }

            if (indices.is_complete()) break;
        }

        return indices;
    }

    bool is_suitable_device(const VkPhysicalDevice& device)
    {
        auto indices = find_queue_families(device);
        return indices.is_complete();
    }

    void select_physical_device(VkInstance& instance, VkPhysicalDevice& device)
    {
        unsigned int num_devices = 0;
        vkEnumeratePhysicalDevices(instance, &num_devices, nullptr);

        if (num_devices == 0)
        {
            throw std::runtime_error {"Available GPUs do not support Vulkan"};
        }

        std::vector<VkPhysicalDevice> available_devices(num_devices);
        vkEnumeratePhysicalDevices(instance, &num_devices, available_devices.data());

        for (const auto& potential_device : available_devices)
        {
            if (is_suitable_device(potential_device))
            {
                device = potential_device;
                break;
            }
        }

        if (device == VK_NULL_HANDLE)
        {
            throw std::runtime_error {"Available GPUs do not support Vulkan"};
        }
    }

    void create_logical_device(VkPhysicalDevice& physical_device, VkDevice& device, VkQueue& queue)
    {
        auto indices = find_queue_families(physical_device);

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
        device_create_info.enabledLayerCount = static_cast<uint32_t> (LAYER_NAMES.size());
        device_create_info.ppEnabledLayerNames = LAYER_NAMES.data();
#else
        createInfo.enabledLayerCount = 0;
#endif
        device_create_info.enabledExtensionCount = 0;

        if (vkCreateDevice(physical_device, &device_create_info, nullptr, &device) != VK_SUCCESS)
        {
            throw std::runtime_error {"Failed to create a logical device"};
        }

        vkGetDeviceQueue(device, indices.m_graphics_family.value(), 0, &queue);
    }
}
