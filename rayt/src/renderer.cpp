#include <rayt/renderer.h>
#include <rayt/config.h>
#include <rayt/checks.h>
#include <rayt/initializers.h>
#include <bootstrap.h>

#define VK_MAJOR_VERSION 1
#define VK_MINOR_VERSION 2
#define VK_PATCH_VERSION 0

#define VK_SHORT_VERSION VK_MAJOR_VERSION, VK_MINOR_VERSION
#define VK_VERSION       VK_MAJOR_VERSION, VK_MINOR_VERSION, VK_PATCH_VERSION

namespace rayt
{
    static vkb::Instance create_instance()
    {
        vkb::InstanceBuilder builder;

        auto instance_result = builder
                .set_app_name("rayt")
#ifdef RAYT_ENABLE_VALIDATION_LAYERS
                .request_validation_layers(true)
#else
                        .request_validation_layers(false)
#endif
                .require_api_version(VK_VERSION)
                .use_default_debug_messenger()
                .build();

        return instance_result.value();
    }

    static vkb::PhysicalDevice choose_physical_device(const vkb::Instance& instance, const VkSurfaceKHR& surface)
    {
        vkb::PhysicalDeviceSelector selector { instance };
        auto device = selector
                .set_minimum_version(VK_SHORT_VERSION)
                .set_surface(surface)
                .select();

        return device.value();
    }

    static vkb::Device create_logical_device(const vkb::PhysicalDevice& physical_device)
    {
        vkb::DeviceBuilder builder { physical_device };
        return builder.build().value();
    }

    static vkb::Swapchain
    create_swapchain(const VkPhysicalDevice& physical_device, const VkDevice& device, const VkSurfaceKHR& surface,
                     const window_t& window)
    {
        vkb::SwapchainBuilder builder { physical_device, device, surface };
        auto vkb_swapchain = builder
                .use_default_format_selection()
                .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
                .set_desired_extent(window.width, window.height)
                .build();

        return vkb_swapchain.value();
    }

    static void create_default_render_pass(VkFormat format, const VkDevice& device, VkRenderPass& render_pass)
    {
        VkAttachmentDescription color_attachment {};
        color_attachment.format = format;
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment_ref {};
        color_attachment_ref.attachment = 0;
        color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_ref;

        VkRenderPassCreateInfo render_pass_info {};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.attachmentCount = 1;
        render_pass_info.pAttachments = &color_attachment;
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;

        VK_SAFE_CALL(vkCreateRenderPass(device, &render_pass_info, nullptr, &render_pass))
    }

    renderer_t::renderer_t(window_t* p_window)
            : m_window_ptr(p_window)
    {
        // Create instance
        auto vkb_instance = create_instance();
        m_instance = vkb_instance.instance;
        m_messenger = vkb_instance.debug_messenger;

        // Create surface
        m_window_ptr->create_surface(m_instance, m_surface);

        // Choose physical device
        auto vkb_physical_device = choose_physical_device(vkb_instance, m_surface);
        m_physical_device = vkb_physical_device.physical_device;

        // Create logical device
        auto vkb_device = create_logical_device(vkb_physical_device);
        m_device = vkb_device.device;

        // Create swapchain
        // TODO: Move to window_t?
        auto vkb_swapchain = create_swapchain(m_physical_device, m_device, m_surface, *m_window_ptr);
        m_swapchain = vkb_swapchain.swapchain;
        m_swapchain_images = vkb_swapchain.get_images().value();
        m_swapchain_image_views = vkb_swapchain.get_image_views().value();
        m_swapchain_image_format = vkb_swapchain.image_format;

        // Setup queues
        m_graphics_queue = vkb_device.get_queue(vkb::QueueType::graphics).value();
        m_graphics_queue_family = vkb_device.get_queue_index(vkb::QueueType::graphics).value();

        // Setup commands
        init_commands();

        // Setup render pass
        create_default_render_pass(m_swapchain_image_format, m_device, m_render_pass);
        init_framebuffer();

        // Setup sync structures
        init_sync_structures();
    }

    void renderer_t::init_commands()
    {
        // Create a command pool
        VK_CREATE_COMMAND_POOL_INFO(pool_info, m_graphics_queue_family, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
        VK_SAFE_CALL(vkCreateCommandPool(m_device, &pool_info, nullptr, &m_command_pool))

        // Create a command buffer
        VK_CREATE_COMMAND_BUFFER_INFO(buf_info, m_command_pool, 1, VK_COMMAND_BUFFER_LEVEL_PRIMARY)
        VK_SAFE_CALL(vkAllocateCommandBuffers(m_device, &buf_info, &m_main_command_buffer))
    }

    void renderer_t::init_framebuffer()
    {
        VkFramebufferCreateInfo fb_info {};
        fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fb_info.pNext = nullptr;
        fb_info.renderPass = m_render_pass;
        fb_info.attachmentCount = 1;
        fb_info.width = m_window_ptr->width;
        fb_info.height = m_window_ptr->height;
        fb_info.layers = 1;

        m_framebuffers = std::vector<VkFramebuffer>(m_swapchain_images.size());
        for (int i = 0; i < m_swapchain_image_views.size(); ++i)
        {
            fb_info.pAttachments = &m_swapchain_image_views[i];
            VK_SAFE_CALL(vkCreateFramebuffer(m_device, &fb_info, nullptr, &m_framebuffers[i]))
        }
    }

    void renderer_t::init_sync_structures()
    {
        // Create fence
        VkFenceCreateInfo fence_info {};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.pNext = nullptr;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VK_SAFE_CALL(vkCreateFence(m_device, &fence_info, nullptr, &m_render_fence));

        // Create semaphores
        VkSemaphoreCreateInfo sem_info {};
        sem_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        sem_info.pNext = nullptr;
        sem_info.flags = 0;

        VK_SAFE_CALL(vkCreateSemaphore(m_device, &sem_info, nullptr, &m_present_semaphore))
        VK_SAFE_CALL(vkCreateSemaphore(m_device, &sem_info, nullptr, &m_render_semaphore))
    }

    void renderer_t::draw()
    {
        ++m_frame_number;

        // Start frame
        VK_SAFE_CALL(vkWaitForFences(m_device, 1, &m_render_fence, true, 1e9))
        VK_SAFE_CALL(vkResetFences(m_device, 1, &m_render_fence));

        unsigned int swapchain_image_index;
        VK_SAFE_CALL(
                vkAcquireNextImageKHR(m_device, m_swapchain, 1e9, m_present_semaphore, nullptr, &swapchain_image_index))

        VK_SAFE_CALL(vkResetCommandBuffer(m_main_command_buffer, 0))

        VkCommandBuffer cmd = m_main_command_buffer;

        VkCommandBufferBeginInfo begin_info {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.pNext = nullptr;
        begin_info.pInheritanceInfo = nullptr;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VK_SAFE_CALL(vkBeginCommandBuffer(cmd, &begin_info))

        // Record draw calls
        VkClearValue clear_value;
        float flash = abs(sin(m_frame_number / 120.0f));
        clear_value.color = {{ 0.6f, 0.6f, flash, 1.0f }};

        VkRenderPassBeginInfo rp_info {};
        rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rp_info.pNext = nullptr;
        rp_info.renderPass = m_render_pass;
        rp_info.renderArea.offset.x = 0;
        rp_info.renderArea.offset.y = 0;
        rp_info.renderArea.extent = {
                (unsigned int) m_window_ptr->width,
                (unsigned int) m_window_ptr->height
        };
        rp_info.framebuffer = m_framebuffers[swapchain_image_index];
        rp_info.clearValueCount = 1;
        rp_info.pClearValues = &clear_value;

        vkCmdBeginRenderPass(cmd, &rp_info, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdEndRenderPass(cmd);

        VK_SAFE_CALL(vkEndCommandBuffer(cmd))

        // Submit the queue
        VkSubmitInfo submit {};
        submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit.pNext = nullptr;

        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        submit.pWaitDstStageMask = &wait_stage;

        submit.waitSemaphoreCount = 1;
        submit.pWaitSemaphores = &m_present_semaphore;

        submit.signalSemaphoreCount = 1;
        submit.pSignalSemaphores = &m_render_semaphore;

        submit.commandBufferCount = 1;
        submit.pCommandBuffers = &cmd;

        VK_SAFE_CALL(vkQueueSubmit(m_graphics_queue, 1, &submit, m_render_fence))

        // Display to the screen
        VkPresentInfoKHR present_info {};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.pNext = nullptr;
        present_info.pSwapchains = &m_swapchain;
        present_info.swapchainCount = 1;
        present_info.pWaitSemaphores = &m_render_semaphore;
        present_info.waitSemaphoreCount = 1;
        present_info.pImageIndices = &swapchain_image_index;

        VK_SAFE_CALL(vkQueuePresentKHR(m_graphics_queue, &present_info))
    }

    renderer_t::~renderer_t()
    {
        // Destroy commands
        vkDestroyCommandPool(m_device, m_command_pool, nullptr);

        // Destroy the swapchain and render pass
        vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
        vkDestroyRenderPass(m_device, m_render_pass, nullptr);
        for (int i = 0; i < m_swapchain_image_views.size(); ++i)
        {
            vkDestroyFramebuffer(m_device, m_framebuffers[i], nullptr);
            vkDestroyImageView(m_device, m_swapchain_image_views[i], nullptr);
        }

        // Destroy device
        vkDestroyDevice(m_device, nullptr);

        // Destroy surface
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

        // Destroy instance
        vkb::destroy_debug_utils_messenger(m_instance, m_messenger, nullptr);
        vkDestroyInstance(m_instance, nullptr);
    }
}
