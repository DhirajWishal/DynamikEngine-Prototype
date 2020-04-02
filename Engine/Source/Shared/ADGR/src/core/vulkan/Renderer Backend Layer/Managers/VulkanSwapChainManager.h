#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_SWAP_CHAIN_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_SWAP_CHAIN_MANAGER_H

#include "Objects/VulkanCoreObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct VulkanSwapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities = {};
				ARRAY<VkSurfaceFormatKHR> formats = {};
				ARRAY<VkPresentModeKHR> presentModes = {};
			};

			class VulkanSwapChainManager {
			public:
				VulkanSwapChainManager() {}
				~VulkanSwapChainManager() {}

				void initialize(POINTER<VulkanCoreObject> core, UI32 width, UI32 height);
				void terminate(POINTER<VulkanCoreObject> core);

				void initializeImageViews(POINTER<VulkanCoreObject> core);
			};

			VulkanSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SWAP_CHAIN_H
