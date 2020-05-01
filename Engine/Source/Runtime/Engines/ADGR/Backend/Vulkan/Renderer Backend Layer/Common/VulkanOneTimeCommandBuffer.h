#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_ONE_TIME_COMMAND_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_ONE_TIME_COMMAND_BUFFER_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanOneTimeCommandBuffer {
			public:
				VulkanOneTimeCommandBuffer(
					VkDevice logicalDevice,
					VkCommandPool commandPool,
					VkQueue processQueue,
					VkQueue utilityQueue);
				VulkanOneTimeCommandBuffer();

				VkCommandBuffer buffer = VK_NULL_HANDLE;

			private:
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue processQueue = VK_NULL_HANDLE;
				VkQueue utilityQueue = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_ONE_TIME_COMMAND_BUFFER_H
