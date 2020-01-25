#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_SWAPCHAIN_H
#define _DYNAMIK_ADGR_VULKAN_SWAPCHAIN_H

#include "backend/vulkan/core/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct swapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities = {};
				std::vector<VkSurfaceFormatKHR> formats = {};
				std::vector<VkPresentModeKHR> presentModes = {};
			};

			struct DMKSwapChainCleanUpInfo {
				std::vector<std::vector<VkBuffer>> uniformBuffers;
				std::vector<std::vector<VkDeviceMemory>> uniformBufferMemories;
				std::vector<VkDescriptorPool> descriptorPools;
				std::vector<VkPipeline> pipelines;
				std::vector<VkPipelineLayout> pipelineLayouts;
			};

			class swapChainManager {
			public:
				swapChainManager() {}
				~swapChainManager() {}

				void init(ADGRVulkanDataContainer* container) ;
				void clear(ADGRVulkanDataContainer* container) ;

				void cleanUp(ADGRVulkanDataContainer* container, DMKSwapChainCleanUpInfo& info);

				void initImageViews(ADGRVulkanDataContainer* container) ;

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
