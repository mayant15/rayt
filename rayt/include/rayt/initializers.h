#pragma once

#define VK_CREATE_COMMAND_POOL_INFO(VARNAME, QUEUE_FAMILY, FLAGS) \
    VkCommandPoolCreateInfo VARNAME {}; \
    VARNAME.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO; \
    VARNAME.pNext = nullptr; \
    VARNAME.queueFamilyIndex = QUEUE_FAMILY; \
    VARNAME.flags = FLAGS;

#define VK_CREATE_COMMAND_BUFFER_INFO(VARNAME, POOL, COUNT, LEVEL) \
    VkCommandBufferAllocateInfo VARNAME {}; \
    VARNAME.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO; \
    VARNAME.pNext = nullptr; \
    VARNAME.commandPool = POOL; \
    VARNAME.commandBufferCount = COUNT; \
    VARNAME.level = LEVEL; \
