#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H

#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanQueue.h"
#include "VulkanFrameBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderObject.h"
#include "VulkanPushConstant.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanCommandBufferInitInfo {
				VulkanDevice device;
				VulkanFrameBuffer frameBuffer;
				VulkanRenderPass renderPass;
				VulkanSwapChain swapChain;

				ARRAY<VulkanRenderObject>* objects;
			};

			class VulkanCommandBuffer {
			public:
				VulkanCommandBuffer() {}
				~VulkanCommandBuffer() {}

				void initializeCommandPool(VulkanInstance instance, VulkanDevice device);
				void initialize(ADGRVulkanCommandBufferInitInfo info);
				void terminate(VulkanDevice device);

				VkCommandPool myCommandPool = VK_NULL_HANDLE;
				ARRAY<VkCommandBuffer> myCommandBuffers;

			private:
				void drawVertex(VkCommandBuffer buffer, I32 index, VulkanRenderObject* object, VkDeviceSize* offsets);
				void drawIndexed(VkCommandBuffer buffer, I32 index, VulkanRenderObject* object, VkDeviceSize* offsets);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H
