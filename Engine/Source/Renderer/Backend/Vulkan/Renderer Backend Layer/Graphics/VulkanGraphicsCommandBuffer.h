#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_GRAPHICS_COMMAND_BUFFER_H
#define _DYNAMIK_RENDERER_VULKAN_GRAPHICS_COMMAND_BUFFER_H

#include "VulkanGraphicsContainers.h"
#include "VulkanGraphicsRenderPass.h"
#include "VulkanGraphicsFrameBuffer.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanGraphicsCommandBufferInitInfo {
				ARRAY<VulkanRenderData> objects;
				VulkanGraphicsSwapChain swapChain;
				VulkanGraphicsRenderPass renderPass;
				VulkanGraphicsFrameBuffer frameBuffer;
				ARRAY<F32> clearValues = {
					(2.0f / 256.0f),
					(8.0f / 256.0f),
					(32.0f / 256.0f),
					(1.00000000f) };
				F32 depthStencilDepth = 1.0f;
				UI32 stencilIndex = 0;

				UI32 count = 3;
			};

			class VulkanGraphicsCommandBuffer {
			public:
				VulkanGraphicsCommandBuffer() {}
				virtual ~VulkanGraphicsCommandBuffer() {}

				void initializeResources(VulkanGraphicsCommandBufferInitResources info);

				virtual void initializeCommandPool();
				void terminateCommandPool();

				virtual void initializeCommandBuffers(VulkanGraphicsCommandBufferInitInfo info);
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

#endif // !_DYNAMIK_RENDERER_VULKAN_COMMAND_BUFFER_H
