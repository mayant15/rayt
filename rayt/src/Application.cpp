#include "Application.h"

#include "Debug.h"
#include "Config.h"

#include <stdexcept>

#if defined(NDEBUG)
    // Disable in release
    constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
    // Enable in debug
    constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif

static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Application::startup()
{
    LOG_INFO("Starting up");
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window =  glfwCreateWindow(_options.WindowWidth, _options.WindowHeight, "rayt", nullptr, nullptr);
    glfwMakeContextCurrent(_window);

    glfwSetKeyCallback(_window, keyboardCallback);

    createVkInstance();
}

void Application::tick()
{
    while(!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
    }
}

void Application::shutdown()
{
    LOG_INFO("Shutting down");
    vkDestroyInstance(_instance, nullptr);
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Application::createVkInstance()
{
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
    createInfo.enabledLayerCount = 0;
    createInfo.pApplicationInfo = &appInfo;
    
    // Get required extensions
    unsigned int nrExtensions;
    const char** extensions = glfwGetRequiredInstanceExtensions(&nrExtensions);
    createInfo.enabledExtensionCount = nrExtensions;
    createInfo.ppEnabledExtensionNames = extensions;
    createInfo.enabledLayerCount = 0;

    // Create instance
    if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
}
