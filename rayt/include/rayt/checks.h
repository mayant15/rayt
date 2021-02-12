#pragma once

#include "logger.h"

#define RAYT_CHECK(x)                           \
    {                                           \
        VkResult err = x;                       \
        if (err)                                \
        {                                       \
            LOG_ERROR("Vulkan error: {}", err); \
            abort();                            \
        }                                       \
    }
