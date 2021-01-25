#pragma once

#include "devices.h"
#include "rayt_export.h"
#include <optional>

namespace rayt
{
    /** @brief Indices for available Vulkan queue families */
    class RAYT_EXPORT queue_family_indices_t
    {
    public:
        std::optional<unsigned int> m_graphics_family;

        explicit queue_family_indices_t(const physical_device_t& device);
        ~queue_family_indices_t() = default;

        [[nodiscard]] inline bool is_complete() const
        {
            return m_graphics_family.has_value();
        }
    };

    class RAYT_EXPORT queue_t
    {
        VkQueue m_queue {};
    public:
//        queue_t(const device_t& device, unsigned int queue_family_index, unsigned int queue_index);
        queue_t() = default;
        ~queue_t() = default;
    };
}
