#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_DEPTH_BUFFER_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_DEPTH_BUFFER_MANAGER_H

#include "VulkanDeviceManager.h"
#include "VulkanCommandBufferManager.h"
#include "VulkanSwapChainManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanDepthBufferContainer {
				VkImage image = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkImageView imageView;
			};

			class VulkanDepthBufferManager {
			public:
				VulkanDepthBufferManager() {}
				~VulkanDepthBufferManager() {}

				ADGRVulkanDepthBufferContainer initialize(POINTER<VulkanCoreObject> core);
				void terminate(POINTER<VulkanCoreObject> core, ADGRVulkanDepthBufferContainer container);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_DEPTH_BUFFER_H
