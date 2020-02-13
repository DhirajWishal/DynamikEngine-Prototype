#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_SWAPCHAIN_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_SWAPCHAIN_MANAGER_H
#ifdef DMK_USE_VULKAN

#include "vulkan/backend/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct swapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities = {};
				std::vector<VkSurfaceFormatKHR> formats = {};
				std::vector<VkPresentModeKHR> presentModes = {};
			};

			class swapChainManager {
			public:
				swapChainManager() {}
				~swapChainManager() {}

				void init(ADGRVulkanDataContainer* container);
				void clear(ADGRVulkanDataContainer* container);

				void initImageViews(ADGRVulkanDataContainer* container);

			private:
				VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
				swapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);
				VkExtent2D chooseSwapExtent(ADGRVulkanDataContainer* container, const VkSurfaceCapabilitiesKHR& capabilities);
				VkExtent2D chooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities);
				VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			};

			swapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);
		}
	}
}

#endif // DMK_USE_VULKAN

#endif	// !_DYNAMIK_ADGR_VULKAN_SWAPCHAIN_H
