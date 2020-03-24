#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H

#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanQueue.h"

#include "core/vulkan/backend/vulkanFormat.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanCommandBuffer {
			public:
				VulkanCommandBuffer() {}
				~VulkanCommandBuffer() {}

				void initializeCommandPool(VulkanInstance instance, VulkanDevice device);
				void initialize(VulkanDevice device, ARRAY<core::vulkanFormat>* formats);
				void terminate(VulkanDevice device);

				VkCommandPool getCommandPool() { return myCommandPool; }
				POINTER<VkCommandPool> getCommandPoolAddress() { return &myCommandPool; }

				ARRAY<VkCommandBuffer> getBuffers() { return myCommandBuffers; }

			private:
				void drawVertex(VkCommandBuffer buffer, I32 index, core::vulkanFormat* format, VkDeviceSize* offsets);
				void drawIndexed(VkCommandBuffer buffer, I32 index, core::vulkanFormat* format, VkDeviceSize* offsets);

				VkCommandPool myCommandPool = VK_NULL_HANDLE;
				ARRAY<VkCommandBuffer> myCommandBuffers;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H
