#include <rayt/pipelines.h>
#include <rayt/logger.h>

namespace rayt
{
    VkPipeline pipeline_builder_t::build(const VkDevice& device, const VkRenderPass& pass)
    {
        VkPipelineViewportStateCreateInfo viewport_state {};
        viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state.pNext = nullptr;
        viewport_state.viewportCount = 1;
        viewport_state.pViewports = &m_viewport;
        viewport_state.scissorCount = 1;
        viewport_state.pScissors = &m_scissor;

        VkPipelineColorBlendStateCreateInfo blend_state {};
        blend_state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        blend_state.pNext = nullptr;
        blend_state.logicOpEnable = VK_FALSE;
        blend_state.logicOp = VK_LOGIC_OP_COPY;
        blend_state.attachmentCount = 1;
        blend_state.pAttachments = &m_color_blend_attachment;

        VkGraphicsPipelineCreateInfo pipeline_info {};
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_info.pNext = nullptr;
        pipeline_info.stageCount = m_shader_stages.size();
        pipeline_info.pStages = m_shader_stages.data();
        pipeline_info.pVertexInputState = &m_vertex_input_info;
        pipeline_info.pInputAssemblyState = &m_input_assembly;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &m_rasterizer;
        pipeline_info.pMultisampleState = &m_multisampling;
        pipeline_info.pColorBlendState = &blend_state;
        pipeline_info.layout = m_pipeline_layout;
        pipeline_info.renderPass = pass;
        pipeline_info.subpass = 0;
        pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

        VkPipeline pipeline;
        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline) != VK_SUCCESS) {
            LOG_ERROR("Failed to create pipeline.");
            return VK_NULL_HANDLE;
        }
        else
        {
            return pipeline;
        }
    }
}
