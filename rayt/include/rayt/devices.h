#pragma once

#include "instance.h"

namespace rayt
{
    class RAYT_EXPORT physical_device_t
    {
    private:
        VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;

    public:
        explicit physical_device_t(VkPhysicalDevice device) : m_physical_device(device) {}
        physical_device_t() = default;
        ~physical_device_t() = default;

        [[nodiscard]] inline const VkPhysicalDevice& get_vk() const { return m_physical_device; }
        [[nodiscard]] bool is_valid() const;
    };

    std::vector<physical_device_t> RAYT_EXPORT enumerate_physical_devices(instance_t& instance);

    class RAYT_EXPORT device_t
    {
        VkDevice m_device = VK_NULL_HANDLE;
        std::vector<const char*> m_validation_layers {};

    public:
        explicit device_t(const physical_device_t& physical_device);
        device_t() = default;
        ~device_t();

        [[nodiscard]] inline const VkDevice& get_vk() const { return m_device; }
        [[nodiscard]] bool is_valid() const { return m_device != VK_NULL_HANDLE; }
    };
}
