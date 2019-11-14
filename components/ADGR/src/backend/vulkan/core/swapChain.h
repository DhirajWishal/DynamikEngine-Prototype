#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_SWAPCHAIN_H
#define _DYNAMIK_ADGR_VULKAN_SWAPCHAIN_H

#include "core/SwapChain.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct DMKSwapChainCleanUpInfo {
				std::vector<std::vector<VkBuffer>> uniformBuffers;
				std::vector<std::vector<VkDeviceMemory>> uniformBufferMemories;
				std::vector<VkDescriptorPool> descriptorPool;
			};

			class swapChain : public SwapChain {
			public:
				swapChain() {}
				~swapChain() {}

				void init(ADGRVulkanDataContainer* container) override;
				void clear(ADGRVulkanDataContainer* container) override;

				void cleanUp(ADGRVulkanDataContainer* container, DMKSwapChainCleanUpInfo& info);

				void initImageViews(ADGRVulkanDataContainer* container) override;

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

#endif	// !_DYNAMIK_ADGR_VULKAN_SWAPCHAIN_H
