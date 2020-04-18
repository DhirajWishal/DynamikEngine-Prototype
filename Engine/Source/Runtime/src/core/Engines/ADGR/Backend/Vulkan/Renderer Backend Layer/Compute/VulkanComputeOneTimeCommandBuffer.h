#pragma once
#ifndef _DYNAMIK_ADGR_COMPUTE_ONE_TIME_COMMAND_BUFFER_H
#define _DYNAMIK_ADGR_COMPUTE_ONE_TIME_COMMAND_BUFFER_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanComputeOneTimeCommandBuffer {
			public:
				VulkanComputeOneTimeCommandBuffer(VkDevice device, VkCommandPool commandPool, VkQueue computeQueue);
				~VulkanComputeOneTimeCommandBuffer();

				VkCommandBuffer buffer = VK_NULL_HANDLE;

			private:
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue computeQueue = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_COMPUTE_ONE_TIME_COMMAND_BUFFER_H
