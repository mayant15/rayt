#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "rayt_export.h"
#include <vector>

namespace rayt
{
    class RAYT_EXPORT instance_t
    {
        VkInstance m_instance {};
        std::vector<const char*> m_validation_layers {};

    public:
        instance_t();
        ~instance_t();

        inline VkInstance& get_vk() { return m_instance; }

    private:
        void check_required_layer_support() const;
    };
}
