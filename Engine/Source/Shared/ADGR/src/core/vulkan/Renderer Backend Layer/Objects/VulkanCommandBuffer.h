#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H

#include "VulkanRenderableObject.h"
#include "VulkanSwapChain.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanCommandBufferInitResources {
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkSurfaceKHR surface = VK_NULL_HANDLE;
			};

			struct ADGRVulkanCommandBufferInitInfo {
				ARRAY<ADGRVulkanRenderData> objects;
				VulkanSwapChain swapChain;
				ARRAY<F32> clearValues = {
					(2.0f / 256.0f),
					(8.0f / 256.0f),
					(32.0f / 256.0f),
					(1.00000000f) };
				F32 depthStencilDepth = 1.0f;
				UI32 stencilIndex = 0;

				UI32 count = 3;
			};

			class VulkanCommandBuffer {
			public:
				VulkanCommandBuffer() {}
				virtual ~VulkanCommandBuffer() {}

				void initializeResources(ADGRVulkanCommandBufferInitResources info);

				virtual void initializeCommandPool();
				void terminateCommandPool();

				virtual void initializeCommandBuffers(ADGRVulkanCommandBufferInitInfo info);
				void terminateCommandBuffers();

				VkCommandPool pool = VK_NULL_HANDLE;
				ARRAY<VkCommandBuffer> buffers;

			protected:
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkSurfaceKHR surface = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H
