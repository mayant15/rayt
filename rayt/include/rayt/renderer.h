#pragma once

#include "types.h"
#include "window.h"

namespace rayt
{
    /** @brief Client interface for the renderer and maintaining state */
    class RAYT_EXPORT renderer_t
    {
        VkInstance m_instance;
        VkDebugUtilsMessengerEXT m_messenger;
        VkSurfaceKHR m_surface {};
        VkPhysicalDevice m_physical_device;
        VkDevice m_device;

        window_t* m_window_ptr = nullptr;

        VkSwapchainKHR m_swapchain;
        VkFormat m_swapchain_image_format;
        std::vector<VkImage> m_swapchain_images;
        std::vector<VkImageView> m_swapchain_image_views;

    public:
        explicit renderer_t(window_t* p_window = nullptr);
        ~renderer_t();

        void draw() {};
    };
}
