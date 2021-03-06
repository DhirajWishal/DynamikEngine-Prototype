#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_GRAPHICS_FRAME_BUFFER_H
#define _DYNAMIK_RENDERER_VULKAN_GRAPHICS_FRAME_BUFFER_H

#include "VulkanGraphicsSwapChain.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanGraphicsFrameBufferInitInfo {
				UI32 bufferCount = 0;
				VkExtent2D swapChainExtent;
				std::vector<VkImageView> swapChainImageViews;

				std::vector<VkImageView> attachments;
				VkRenderPass renderPass = VK_NULL_HANDLE;
			};

			class VulkanGraphicsFrameBuffer {
			public:
				VulkanGraphicsFrameBuffer() {}
				virtual ~VulkanGraphicsFrameBuffer() {}

				virtual void initialize(VkDevice logicalDevice, VulkanGraphicsFrameBufferInitInfo info);
				virtual void terminate(VkDevice logicalDevice);

				static VkFramebuffer createFrameBuffer(
					VkDevice logicalDevice,
					VkRenderPass renderPass,
					std::vector<VkImageView> attachments,
					VkExtent2D extent,
					UI32 layerCount = 1);
				static void terminateFrameBuffer(VkDevice logicalDevice, VkFramebuffer buffer);

				std::vector<VkFramebuffer> buffers;
				UI32 frameWidth = 0;
				UI32 frameHeight = 0;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_FRAME_BUFFER_H
