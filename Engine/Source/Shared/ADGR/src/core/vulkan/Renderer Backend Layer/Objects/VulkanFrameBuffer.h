#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_FRAME_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_FRAME_BUFFER_H

#include "VulkanSwapChain.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanRenderPassInitInfo {
				UI32 destinationSubpass = 0;
				VkAccessFlags accessFlags = 0;
				B1 overrideDependencies = false;

				ARRAY<VkAttachmentDescription> attachments;

				ARRAY<VkSubpassDescription> subPasses;
				ARRAY<VkSubpassDependency> additionalSubPassDependencies;
			};

			struct ADGRVulkanFrameBufferInitInfo {
				UI32 bufferCount = 0;
				VkExtent2D swapChainExtent;
				ARRAY<VkImageView> swapChainImageViews;

				ARRAY<VkImageView> attachments;
				VkRenderPass overrideRenderPass = VK_NULL_HANDLE;
			};

			class VulkanFrameBuffer {
			public:
				VulkanFrameBuffer() {}
				virtual ~VulkanFrameBuffer() {}

				virtual void initializeRenderPass(VkDevice logicalDevice, ADGRVulkanRenderPassInitInfo info);
				virtual void initializeFrameBuffer(VkDevice logicalDevice, ADGRVulkanFrameBufferInitInfo info);

				virtual void terminate(VkDevice logicalDevice);

				VkRenderPass renderPass = VK_NULL_HANDLE;
				ARRAY<VkFramebuffer> buffers;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_FRAME_BUFFER_H
