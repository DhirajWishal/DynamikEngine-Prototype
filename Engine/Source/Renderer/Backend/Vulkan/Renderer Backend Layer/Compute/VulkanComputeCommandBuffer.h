#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_COMPUTE_COMMAND_BUFFER_H
#define _DYNAMIK_RENDERER_VULKAN_COMPUTE_COMMAND_BUFFER_H

#include "VulkanComputeObject.h"
#include "VulkanComputePipeline.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanComputeCommandBufferInitInfo {
				std::vector<VulkanComputeData> computeDatas;
			};

			class VulkanComputeCommandBuffer {
			public:
				VulkanComputeCommandBuffer() {}
				virtual ~VulkanComputeCommandBuffer() {}

				virtual void initializeCommandPool(VkDevice device, UI32 computeFamilyIndex);
				virtual void initializeCommandBuffers(VkDevice device, VulkanComputeCommandBufferInitInfo info);

				UI32 computeFamilyIndex = 0;
				VkCommandPool pool = VK_NULL_HANDLE;
				std::vector<VkCommandBuffer> buffers;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_COMPUTE_COMMAND_BUFFER_H
