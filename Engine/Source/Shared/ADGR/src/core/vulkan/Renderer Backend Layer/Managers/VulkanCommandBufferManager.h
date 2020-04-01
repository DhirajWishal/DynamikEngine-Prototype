#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_MANAGER_H

#include "VulkanSwapChainManager.h"
#include "VulkanFrameBufferManager.h"
#include "VulkanRenderPassManager.h"
#include "VulkanPushConstantManager.h"

#include "Objects/VulkanRenderObject.h"
#include "Objects/VulkanCoreObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanCommandBufferInitInfo {
				POINTER<ARRAY<VulkanRenderObject>> objects;
			};

			class VulkanCommandBufferManager {
			public:
				VulkanCommandBufferManager() {}
				~VulkanCommandBufferManager() {}

				void initializeCommandPool(POINTER<VulkanCoreObject> core);
				void initialize(POINTER<VulkanCoreObject> core, ADGRVulkanCommandBufferInitInfo info);
				void terminate(POINTER<VulkanCoreObject> core);

			private:
				void drawVertex(VkCommandBuffer buffer, I32 index, VulkanRenderObject* object, VkDeviceSize* offsets);
				void drawIndexed(VkCommandBuffer buffer, I32 index, VulkanRenderObject* object, VkDeviceSize* offsets);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H
