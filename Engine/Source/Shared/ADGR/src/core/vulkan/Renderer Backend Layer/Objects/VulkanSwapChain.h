#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_SWAP_CHAIN_H
#define _DYNAMIK_ADGR_VULKAN_SWAP_CHAIN_H

#include "VulkanDevice.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct VulkanSwapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities = {};
				ARRAY<VkSurfaceFormatKHR> formats = {};
				ARRAY<VkPresentModeKHR> presentModes = {};
			};

			class VulkanSwapChain {
			public:
				VulkanSwapChain() {}
				~VulkanSwapChain() {}

				void initialize(VulkanInstance instance, VulkanDevice device, UI32 width, UI32 height);
				void terminate(VulkanDevice device);

				void initializeImageViews(VulkanDevice device);

				VkSwapchainKHR mySwapChain = VK_NULL_HANDLE;
				ARRAY<VkImage> mySwapChainImages;
				ARRAY<VkImageView> mySwapChainImageViews;
				VkFormat mySwapChainImageFormat;
				VkExtent2D mySwapChainExtent;
			};

			VulkanSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SWAP_CHAIN_H
