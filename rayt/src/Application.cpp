#include "Application.h"

#include "Debug.h"
#include "Config.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <optional>

#if defined(NDEBUG)
#define ENABLE_VALIDATION_LAYERS false
#else
#define ENABLE_VALIDATION_LAYERS true
#endif

class queue_family_indices_t
{
public:
    std::optional<unsigned int> graphics_family;

    bool is_complete()
    {
        return graphics_family.has_value();
    }
};

static const std::vector<const char*> VALIDATION_LAYER_NAMES = {
    "VK_LAYER_KHRONOS_validation"
};

static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static bool checkValidationLayerSupport()
{
    unsigned int nrLayers;
    vkEnumerateInstanceLayerProperties(&nrLayers, nullptr);

    std::vector<VkLayerProperties> layers(nrLayers);
    vkEnumerateInstanceLayerProperties(&nrLayers, layers.data());

    // Check if the required layers are available
    for (const auto* layerName : VALIDATION_LAYER_NAMES)
    {
        bool isPresent = false;
        for (const auto& layer : layers)
        {
            if (strcmp(layerName, layer.layerName) == 0)
            {
                isPresent = true;
            }
        }
        if (!isPresent)
        {
            LOG_WARN("Validation layer not available: {}", layerName);
            return false;
        }
    }

    return true;
}

static void createVkInstance(VkInstance& instance)
{
    if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport())
    {
        throw std::runtime_error{ "Requested validation layers are unavailable" };
    }

    // Setup application information
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pEngineName = "No Engine";
    appInfo.pApplicationName = "rayt";
    appInfo.applicationVersion = VK_MAKE_VERSION(RAYT_VERSION_MAJOR, RAYT_VERSION_MINOR, RAYT_VERSION_PATCH);
    appInfo.engineVersion = VK_MAKE_VERSION(RAYT_VERSION_MAJOR, RAYT_VERSION_MINOR, RAYT_VERSION_PATCH);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    // Setup instance information
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if (ENABLE_VALIDATION_LAYERS)
    {
        createInfo.enabledLayerCount = VALIDATION_LAYER_NAMES.size();
        createInfo.ppEnabledLayerNames = VALIDATION_LAYER_NAMES.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    // Get required extensions
    unsigned int nrExtensions;
    const char** extensions = glfwGetRequiredInstanceExtensions(&nrExtensions);
    createInfo.enabledExtensionCount = nrExtensions;
    createInfo.ppEnabledExtensionNames = extensions;

    // Create instance
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
}

static queue_family_indices_t findQueueFamilies(const VkPhysicalDevice& device)
{
    queue_family_indices_t indices;

    unsigned int num_queue_families = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &num_queue_families, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(num_queue_families);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &num_queue_families, queue_families.data());

    unsigned int i = 0;
    for (const auto& family : queue_families)
    {
        if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphics_family = i;
        }

        if (indices.is_complete())
        {
            break;
        }

        i++;
    }

    return indices;
}

static bool isSuitableDevice(const VkPhysicalDevice& physicalDevice)
{
    queue_family_indices_t indices = findQueueFamilies(physicalDevice);
    return indices.is_complete();
}

static void pickPhysicalDevice(const VkInstance& instance, VkPhysicalDevice& device)
{
    unsigned int nrDevices = 0;
    vkEnumeratePhysicalDevices(instance, &nrDevices, nullptr);

    if (nrDevices == 0)
    {
        throw std::runtime_error{ "Available GPUs do not support Vulkan" };
    }

    std::vector<VkPhysicalDevice> devices(nrDevices);
    vkEnumeratePhysicalDevices(instance, &nrDevices, devices.data());

    for (const auto& device_ : devices)
    {
        if (isSuitableDevice(device_))
        {
            device = device_;
            break;
        }
    }

    if (device == VK_NULL_HANDLE)
    {
        throw std::runtime_error{ "Available GPUs do not support Vulkan" };
    }
}

static void createLogicalDevice(VkPhysicalDevice& physical_device, VkDevice& device, VkQueue& queue)
{
    static float queuePriority = 1.0f;

    queue_family_indices_t indices = findQueueFamilies(physical_device);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphics_family.has_value();
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    // Device validation layers
    createInfo.enabledExtensionCount = 0;
    if (ENABLE_VALIDATION_LAYERS)
    {
        createInfo.enabledLayerCount = VALIDATION_LAYER_NAMES.size();
        createInfo.ppEnabledLayerNames = VALIDATION_LAYER_NAMES.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physical_device, &createInfo, nullptr, &device) != VK_SUCCESS)
    {
        throw std::runtime_error{ "Failed to create a logical device" };
    }

    // vkGetDeviceQueue(device, indices.graphics_family.value(), 0, &queue);
}

application_t::application_t()
{
    LOG_INFO("Starting up");

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    p_window =  glfwCreateWindow(RAYT_WINDOW_WIDTH, RAYT_WINDOW_HEIGHT, "rayt", nullptr, nullptr);

    glfwSetKeyCallback(p_window, keyboardCallback);

    createVkInstance(instance);
    pickPhysicalDevice(instance, physical_device);
    createLogicalDevice(physical_device, device, graphics_queue);
}

void application_t::tick()
{
    while(!glfwWindowShouldClose(p_window))
    {
        glfwPollEvents();
    }
}

application_t::~application_t()
{
    LOG_INFO("Shutting down");

    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
    
    glfwDestroyWindow(p_window);
    glfwTerminate();
}
