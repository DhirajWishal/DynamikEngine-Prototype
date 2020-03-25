#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_DEPTH_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_DEPTH_BUFFER_H

#include "VulkanDevice.h"
#include "VulkanQueue.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSwapChain.h"
#include "VulkanGlobalVariables.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanDepthBufferInitInfo {
				VulkanDevice device;
				VulkanQueue queue;
				VulkanCommandBuffer commandBuffer;
				VulkanSwapChain swapChain;
				POINTER<VulkanGlobalVariables> global;
			};

			class VulkanDepthBuffer {
			public:
				VulkanDepthBuffer() {}
				~VulkanDepthBuffer() {}

				void initialize(ADGRVulkanDepthBufferInitInfo info);
				void terminate(VulkanDevice device);

				VkImage image = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkImageView imageView;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_DEPTH_BUFFER_H
