#pragma once

#include "types.h"

namespace rayt::detail
{
    inline VkCommandPoolCreateInfo
    create_command_pool_info(VkCommandPoolCreateFlags flags, unsigned int queue_family_index)
    {
        VkCommandPoolCreateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        info.pNext = nullptr;
        info.flags = flags;
        info.queueFamilyIndex = queue_family_index;
        return info;
    }

    inline VkCommandBufferAllocateInfo
    create_command_buffer_info(VkCommandPool pool, unsigned int command_buffer_count, VkCommandBufferLevel level)
    {
        VkCommandBufferAllocateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        info.pNext = nullptr;
        info.commandPool = pool;
        info.level = level;
        info.commandBufferCount = command_buffer_count;
        return info;
    }

    inline VkPipelineShaderStageCreateInfo
    create_pipeline_shader_stage_info(VkShaderStageFlagBits stage, VkShaderModule module)
    {
        VkPipelineShaderStageCreateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        info.pNext = nullptr;
        info.stage = stage;
        info.module = module;
        info.pName = "main";
        return info;
    }

    inline VkPipelineVertexInputStateCreateInfo
    create_pipeline_vertex_input_state_info()
    {
        VkPipelineVertexInputStateCreateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        info.pNext = nullptr;
        info.vertexBindingDescriptionCount = 0;
        info.vertexAttributeDescriptionCount = 0;
        return info;
    }

    inline VkPipelineInputAssemblyStateCreateInfo
    create_pipeline_input_assembly_state_info(VkPrimitiveTopology topology)
    {
        VkPipelineInputAssemblyStateCreateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        info.pNext = nullptr;
        info.topology = topology;
        info.primitiveRestartEnable = VK_FALSE;
        return info;
    }

    inline VkPipelineRasterizationStateCreateInfo
    create_pipeline_rasterization_state_info(VkPolygonMode polygonMode)
    {
        VkPipelineRasterizationStateCreateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        info.pNext = nullptr;
        info.depthClampEnable = VK_FALSE;
        info.rasterizerDiscardEnable = VK_FALSE;
        info.polygonMode = polygonMode;
        info.lineWidth = 1.0f;
        info.cullMode = VK_CULL_MODE_NONE;
        info.frontFace = VK_FRONT_FACE_CLOCKWISE;
        info.depthBiasEnable = VK_FALSE;
        info.depthBiasConstantFactor = 0.0f;
        info.depthBiasClamp = 0.0f;
        info.depthBiasSlopeFactor = 0.0f;
        return info;
    }

    inline VkPipelineMultisampleStateCreateInfo
    create_pipeline_multisample_state_info()
    {
        VkPipelineMultisampleStateCreateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        info.pNext = nullptr;
        info.sampleShadingEnable = VK_FALSE;
        info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        info.minSampleShading = 1.0f;
        info.pSampleMask = nullptr;
        info.alphaToCoverageEnable = VK_FALSE;
        info.alphaToOneEnable = VK_FALSE;
        return info;
    }

    inline VkPipelineColorBlendAttachmentState
    create_pipeline_color_blend_attachment_state()
    {
        VkPipelineColorBlendAttachmentState state {};
        state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                               VK_COLOR_COMPONENT_A_BIT;
        state.blendEnable = VK_FALSE;
        return state;
    }

    inline VkPipelineLayoutCreateInfo
    create_pipeline_layout_info()
    {
        VkPipelineLayoutCreateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        info.pNext = nullptr;
        info.flags = 0;
        info.setLayoutCount = 0;
        info.pSetLayouts = nullptr;
        info.pushConstantRangeCount = 0;
        info.pPushConstantRanges = nullptr;
        return info;
    }
}
