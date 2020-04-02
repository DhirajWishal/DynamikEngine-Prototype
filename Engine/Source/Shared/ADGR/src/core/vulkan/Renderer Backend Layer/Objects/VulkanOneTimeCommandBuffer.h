#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_ONE_TIME_COMMAND_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_ONE_TIME_COMMAND_BUFFER_H

#include "VulkanCommandBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanOneTimeCommandBuffer {
			public:
				VulkanOneTimeCommandBuffer(VulkanDevice device, VulkanCommandBuffer commandBuffer, VulkanQueue queue, UI32 count = 1);
				~VulkanOneTimeCommandBuffer();

				ARRAY<VkCommandBuffer> myCommandBuffers;
			private:
				VulkanDevice myDevice;
				VulkanCommandBuffer myCommandBuffer;
				VulkanQueue myQueue;

				UI32 myCount = 0;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_ONE_TIME_COMMAND_BUFFER_H
