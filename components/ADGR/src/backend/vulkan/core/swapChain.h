#pragma once

#include "core/SwapChain.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKSwapChainCleanUpInfo {
				std::vector<VkBuffer> uniformBuffers;
				std::vector<VkDeviceMemory> uniformBufferMemories;
				VkDescriptorPool descriptorPool;
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
