#pragma once

#include <rayt/rayt_export.h>
#include <optional>

namespace rayt
{
    /** @brief Indices for available Vulkan queue families */
    class queue_family_indices_t
    {
    public:
        std::optional<unsigned int> m_graphics_family;

        [[nodiscard]] inline bool is_complete() const
        {
            return m_graphics_family.has_value();
        }
    };
}
