#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H

#include "VulkanCommandBuffer.h"
#include "VulkanQueue.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanCommandBuffer;

			class VulkanIndexBuffer {
			public:
				VulkanIndexBuffer() {}
				~VulkanIndexBuffer() {}

				void initialize(VulkanDevice device, VulkanCommandBuffer commandBuffer, VulkanQueue queue, ARRAY<UI32>* indexes);
				void terminate(VulkanDevice device);

				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
				UI32 count = 0;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H
