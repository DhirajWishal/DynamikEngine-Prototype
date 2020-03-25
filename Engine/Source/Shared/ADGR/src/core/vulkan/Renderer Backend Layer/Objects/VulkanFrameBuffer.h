#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_FRAME_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_FRAME_BUFFER_H

#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanFrameBufferInitInfo {
				VulkanDevice device;
				VulkanSwapChain swapChain;
				VulkanRenderPass renderPass;

				ARRAY<VkImageView> additionalAttachments;
			};

			class VulkanFrameBuffer {
			public:
				VulkanFrameBuffer() {}
				~VulkanFrameBuffer() {}

				void initialize(ADGRVulkanFrameBufferInitInfo info);
				void terminate(VulkanDevice device);

				ARRAY<VkFramebuffer> frameBuffers;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_FRAME_BUFFER_H
