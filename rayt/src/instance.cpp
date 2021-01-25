#include <rayt/instance.h>
#include <rayt/config.h>
#include <rayt/logger.h>

#include <vector>
#include <stdexcept>

#define RAYT_VERSION \
    VK_MAKE_VERSION(RAYT_VERSION_MAJOR, RAYT_VERSION_MINOR, RAYT_VERSION_PATCH)

namespace rayt
{
    instance_t::instance_t()
    {
#ifdef RAYT_ENABLE_VALIDATION_LAYERS
        m_validation_layers = { RAYT_REQUIRED_VALIDATION_LAYERS };
        check_required_layer_support();
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
        instance_create_info.enabledLayerCount = static_cast<uint32_t> (m_validation_layers.size());
        instance_create_info.ppEnabledLayerNames = m_validation_layers.data();
#else
        createInfo.enabledLayerCount = 0;
#endif

        // Get required extensions
        unsigned int num_extensions;
        const char** extensions = glfwGetRequiredInstanceExtensions(&num_extensions);
        instance_create_info.enabledExtensionCount = num_extensions;
        instance_create_info.ppEnabledExtensionNames = extensions;

        // Create instance
        if (vkCreateInstance(&instance_create_info, nullptr, &m_instance) != VK_SUCCESS)
        {
            throw std::runtime_error {"Failed to create Vulkan instance"};
        }
    }

    instance_t::~instance_t()
    {
        vkDestroyInstance(m_instance, nullptr);
    }

    void instance_t::check_required_layer_support() const
    {
        unsigned int num_layers;
        vkEnumerateInstanceLayerProperties(&num_layers, nullptr);

        std::vector<VkLayerProperties> available_layers(num_layers);
        vkEnumerateInstanceLayerProperties(&num_layers, available_layers.data());

        // Check if the required layers are available
        for (const auto* requested_layer : m_validation_layers)
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
                throw std::runtime_error {"Requested validation layers are unavailable"};
            }
        }
    }
}
