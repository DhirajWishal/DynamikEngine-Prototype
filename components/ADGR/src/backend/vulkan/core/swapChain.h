#pragma once

#include "core/SwapChain.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKSwapChainCleanUpInfo {
				std::vector<VkBuffer> uniformBuffers;
				std::vector<VkDeviceMemory> uniformBufferMemories;
			};

			class swapChain : public SwapChain {
			public:
				swapChain() {}
				~swapChain() {}

				void init() override;
				void clear() override;

				void cleanUp(DMKSwapChainCleanUpInfo info);

				void initImageViews() override;

				VkSwapchainKHR getSwapChainCpy() { return *m_swapChain; }

			private:
				VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
				swapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);
				VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
				VkExtent2D chooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities);
				VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

				VkDevice* m_device = &device;
				VkSwapchainKHR* m_swapChain = &swapchain;
				VkSurfaceCapabilitiesKHR* m_surfaceCaps = &surfaceCapabilities;
				VkExtent2D* m_swapChainExtent = &swapChainExtent;
				VkFormat* m_swapChainImageFormat = &swapChainImageFormat;

				std::vector<VkImage>* m_swapChainImages = &swapChainImages;
				std::vector<VkImageView>* m_swapChainImageViews = &swapChainImageViews;
			};

			swapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);
		}
	}
}