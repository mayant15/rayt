#pragma once

#include "logger.h"

#define VK_SAFE_CALL(function)                  \
    {                                           \
        VkResult err = function;                \
        if (err)                                \
        {                                       \
            LOG_ERROR("Vulkan error: {}", err); \
            abort();                            \
        }                                       \
    }
