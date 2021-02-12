#pragma once

#include "types.h"

namespace rayt
{
    class RAYT_EXPORT surface_t
    {
        VkSurfaceKHR m_surface{};
        VkInstance* m_instance_ptr = nullptr;

    public:
        surface_t(VkInstance* instance_ptr, void* window_ptr);
        surface_t() = default;
        ~surface_t();
    };
}
