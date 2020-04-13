#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_COMMAND_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_COMMAND_BUFFER_H

#include "VulkanComputeObject.h"
#include "VulkanComputePipeline.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanComputeCommandBufferInitInfo {
				UI32 count = 0;
				ARRAY<ADGRVulkanComputeData> computeDatas;
			};

			class VulkanComputeCommandBuffer {
			public:
				VulkanComputeCommandBuffer() {}
				virtual ~VulkanComputeCommandBuffer() {}

				virtual void initializeCommandPool(VkDevice device, UI32 computeFamilyIndex);
				virtual void initializeCommandBuffers(VkDevice device, ADGRVulkanComputeCommandBufferInitInfo info);

				UI32 computeFamilyIndex = 0;
				VkCommandPool pool = VK_NULL_HANDLE;
				ARRAY<VkCommandBuffer> buffers;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_COMMAND_BUFFER_H
