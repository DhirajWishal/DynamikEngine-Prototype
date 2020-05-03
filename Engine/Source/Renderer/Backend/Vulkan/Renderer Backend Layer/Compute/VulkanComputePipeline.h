#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_COMPUTE_PIPELINE_H
#define _DYNAMIK_RENDERER_VULKAN_COMPUTE_PIPELINE_H

#include "../Graphics/VulkanGraphicsPipeline.h"
#include "VulkanComputeShader.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanComputePipelineInitInfo {
				VulkanComputeShader computeShader;
				VkPipelineCache pipelineCache = VK_NULL_HANDLE;
			};

			class VulkanComputePipeline : public VulkanGraphicsPipeline {
			public:
				VulkanComputePipeline() {}
				virtual ~VulkanComputePipeline() {}

				virtual void initializePipelineLayout(VkDevice device, VulkanGraphicsPipelineLayoutInitInfo info) override;
				virtual void initializePipeline(VkDevice device, VulkanComputePipelineInitInfo info);
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_COMPUTE_PIPELINE_H
