#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_PIPELINE_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_PIPELINE_H

#include "../Objects/VulkanPipeline.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanComputePipelineInitInfo {
				VkPipelineShaderStageCreateInfo computeShaderStage;
				VkPipelineCache pipelineCache = VK_NULL_HANDLE;
			};

			class VulkanComputePipeline : public VulkanPipeline {
			public:
				VulkanComputePipeline() {}
				virtual ~VulkanComputePipeline() {}

				virtual void initializePipelineLayout(VkDevice device, ADGRVulkanPipelineLayoutInitInfo info) override;
				virtual void initializePipeline(VkDevice device, ADGRVulkanComputePipelineInitInfo info);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_PIPELINE_H
