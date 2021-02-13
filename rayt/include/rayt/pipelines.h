#pragma once

#include "types.h"

namespace rayt
{
    class pipeline_builder_t
    {
    public:
        std::vector<VkPipelineShaderStageCreateInfo> m_shader_stages;
        VkPipelineVertexInputStateCreateInfo m_vertex_input_info;
        VkPipelineInputAssemblyStateCreateInfo m_input_assembly;

        VkViewport m_viewport;
        VkRect2D m_scissor;

        VkPipelineRasterizationStateCreateInfo m_rasterizer;
        VkPipelineColorBlendAttachmentState m_color_blend_attachment;
        VkPipelineMultisampleStateCreateInfo m_multisampling;

        VkPipelineLayout m_pipeline_layout;

    public:
        VkPipeline build(const VkDevice& device, const VkRenderPass& pass);
    };
}
