#pragma once

#include "logger.h"

#define VK_SAFE_CALL(x)                         \
    {                                           \
        VkResult err = x;                       \
        if (err)                                \
        {                                       \
            LOG_ERROR("Vulkan error: {}", err); \
            abort();                            \
        }                                       \
    }
