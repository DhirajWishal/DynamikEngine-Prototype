#include "adgrafx.h"
#include "VulkanComputePipeline.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {		
			void VulkanComputePipeline::initializePipelineLayout(VkDevice device, ADGRVulkanGraphicsPipelineLayoutInitInfo info)
			{
				// initialize the pipeline layout
				VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
				pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pipelineLayoutInfo.setLayoutCount = info.layouts.size();
				pipelineLayoutInfo.pSetLayouts = info.layouts.data();

				if (info.pushConstantRanges.size())
				{
					pipelineLayoutInfo.pushConstantRangeCount = info.pushConstantRanges.size();
					pipelineLayoutInfo.pPushConstantRanges = info.pushConstantRanges.data();
				}
				else
				{
					pipelineLayoutInfo.pushConstantRangeCount = 0;
					pipelineLayoutInfo.pPushConstantRanges = VK_NULL_HANDLE;
				}

				// create the pipeline layout
				if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &layout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create compute pipeline layout!");
			}

			void VulkanComputePipeline::initializePipeline(VkDevice device, ADGRVulkanComputePipelineInitInfo info)
			{
				VkComputePipelineCreateInfo pipelineInfo;
				pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
				pipelineInfo.pNext = VK_NULL_HANDLE;
				pipelineInfo.flags = VK_NULL_HANDLE;
				pipelineInfo.stage = info.computeShader.stageCreateInfo;
				pipelineInfo.layout = layout;
				pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
				pipelineInfo.basePipelineIndex = VK_NULL_HANDLE;

				// create the pipeline
				if (vkCreateComputePipelines(device, info.pipelineCache, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create compute pipeline!");
			}
		}
	}
}