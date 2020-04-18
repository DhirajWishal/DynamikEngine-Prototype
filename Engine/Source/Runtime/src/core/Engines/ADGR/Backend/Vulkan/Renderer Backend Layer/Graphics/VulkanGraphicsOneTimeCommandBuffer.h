#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_ONE_TIME_COMMAND_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_ONE_TIME_COMMAND_BUFFER_H

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanGraphicsOneTimeCommandBuffer {
			public:
				VulkanGraphicsOneTimeCommandBuffer(
					VkDevice logicalDevice,
					VkCommandPool commandPool,
					VkQueue graphicsQueue,
					VkQueue presentQueue,
					UI32 count = 1);
				~VulkanGraphicsOneTimeCommandBuffer();

				ARRAY<VkCommandBuffer> myCommandBuffers;
			private:
				VkDevice myDevice;
				VkCommandPool myCommandPool;
				VkQueue myGraphcisQueue;
				VkQueue myPresentQueue;

				UI32 myCount = 0;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_ONE_TIME_COMMAND_BUFFER_H