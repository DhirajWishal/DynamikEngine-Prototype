#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_SWAPCHAIN_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_SWAPCHAIN_MANAGER_H
#ifdef DMK_USE_VULKAN

#include "core/vulkan/backend/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct swapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities = {};
				ARRAY<VkSurfaceFormatKHR> formats = {};
				ARRAY<VkPresentModeKHR> presentModes = {};
			};

			class swapChainManager {
			public:
				swapChainManager() {}
				~swapChainManager() {}

				void init(ADGRVulkanDataContainer* container);
				void clear(ADGRVulkanDataContainer* container);

				void initImageViews(ADGRVulkanDataContainer* container);

			private:
				VkSurfaceFormatKHR chooseSwapSurfaceFormat(const ARRAY<VkSurfaceFormatKHR>& availableFormats);
				swapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);
				VkExtent2D chooseSwapExtent(ADGRVulkanDataContainer* container, const VkSurfaceCapabilitiesKHR& capabilities);
				VkExtent2D chooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities);
				VkPresentModeKHR chooseSwapPresentMode(const ARRAY<VkPresentModeKHR>& availablePresentModes);
			};

			swapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);
		}
	}
}

#endif // DMK_USE_VULKAN

#endif	// !_DYNAMIK_ADGR_VULKAN_SWAPCHAIN_H
