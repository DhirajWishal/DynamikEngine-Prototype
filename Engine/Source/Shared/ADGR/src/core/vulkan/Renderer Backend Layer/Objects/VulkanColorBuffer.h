#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COLOR_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_COLOR_BUFFER_H

#include "VulkanCommandBuffer.h"
#include "VulkanSwapChain.h"
#include "VulkanGlobalVariables.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanColorBufferInitInfo {
				VulkanDevice device;
				VulkanQueue queue;
				VulkanCommandBuffer commandBuffer;
				VulkanSwapChain swapChain;
				POINTER<VulkanGlobalVariables> global;
			};

			class VulkanColorBuffer {
			public:
				VulkanColorBuffer() {}
				~VulkanColorBuffer() {}

				void initialize(ADGRVulkanColorBufferInitInfo info);
				void terminate(VulkanDevice device);

				VkImage image = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkImageView imageView;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COLOR_BUFFER_H
