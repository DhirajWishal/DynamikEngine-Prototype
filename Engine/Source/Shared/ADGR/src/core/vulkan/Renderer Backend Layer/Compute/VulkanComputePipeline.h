#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_PIPELINE_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_PIPELINE_H

#include "../Graphics/VulkanGraphicsPipeline.h"
#include "VulkanComputeShader.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanComputePipelineInitInfo {
				VulkanComputeShader computeShader;
				VkPipelineCache pipelineCache = VK_NULL_HANDLE;
			};

			class VulkanComputePipeline : public VulkanGraphicsPipeline {
			public:
				VulkanComputePipeline() {}
				virtual ~VulkanComputePipeline() {}

				virtual void initializePipelineLayout(VkDevice device, ADGRVulkanGraphicsPipelineLayoutInitInfo info) override;
				virtual void initializePipeline(VkDevice device, ADGRVulkanComputePipelineInitInfo info);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_PIPELINE_H
