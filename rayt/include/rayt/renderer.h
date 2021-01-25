#pragma once

#include "instance.h"
#include "queues.h"
#include "devices.h"
#include "rayt_export.h"
#include "surface.h"

#include <vector>

namespace rayt
{
    /** @brief Client interface for the renderer and maintaining state */
    class RAYT_EXPORT renderer_t
    {
        // NOTE: Order of declaration matters. Cleanup will be performed in reverse order.
        // See https://stackoverflow.com/questions/2254263/order-of-member-constructor-and-destructor-calls

        instance_t m_instance {};
        surface_t m_surface;
        physical_device_t m_physical_device;
        device_t m_device;
        queue_t m_queue;

    public:
        renderer_t();
        ~renderer_t() = default;
    };
}
