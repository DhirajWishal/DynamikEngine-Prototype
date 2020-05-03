#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_GRAPHICS_FRAME_BUFFER_H
#define _DYNAMIK_RENDERER_VULKAN_GRAPHICS_FRAME_BUFFER_H

#include "VulkanGraphicsSwapChain.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanRenderPassInitInfo {
				UI32 destinationSubpass = 0;
				VkAccessFlags accessFlags = 0;
				B1 overrideDependencies = false;

				ARRAY<VkAttachmentDescription> attachments;

				ARRAY<VkSubpassDescription> subPasses;
				ARRAY<VkSubpassDependency> additionalSubPassDependencies;
			};

			struct VulkanGraphicsFrameBufferInitInfo {
				UI32 bufferCount = 0;
				VkExtent2D swapChainExtent;
				ARRAY<VkImageView> swapChainImageViews;

				ARRAY<VkImageView> attachments;
				VkRenderPass overrideRenderPass = VK_NULL_HANDLE;
			};

			class VulkanGraphicsFrameBuffer {
			public:
				VulkanGraphicsFrameBuffer() {}
				virtual ~VulkanGraphicsFrameBuffer() {}

				virtual void initializeRenderPass(VkDevice logicalDevice, VulkanRenderPassInitInfo info);
				virtual void initializeFrameBuffer(VkDevice logicalDevice, VulkanGraphicsFrameBufferInitInfo info);

				virtual void terminate(VkDevice logicalDevice);

				static VkFramebuffer createFrameBuffer(
					VkDevice logicalDevice,
					VkRenderPass renderPass,
					ARRAY<VkImageView> attachments,
					VkExtent2D extent,
					UI32 layerCount = 1);
				static void terminateFrameBuffer(VkDevice logicalDevice, VkFramebuffer buffer);

				VkRenderPass renderPass = VK_NULL_HANDLE;
				ARRAY<VkFramebuffer> buffers;
				UI32 frameWidth = 0;
				UI32 frameHeight = 0;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_FRAME_BUFFER_H
