#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_GRAPHICS_SWAP_CHAIN_H
#define _DYNAMIK_RENDERER_VULKAN_GRAPHICS_SWAP_CHAIN_H

#include <vulkan/vulkan.h>
#include "Public/Array.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanGraphicsSwapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities = {};
				ARRAY<VkSurfaceFormatKHR> formats = {};
				ARRAY<VkPresentModeKHR> presentModes = {};
			};

			class VulkanGraphicsSwapChain {
			public:
				VulkanGraphicsSwapChain() {}
				~VulkanGraphicsSwapChain() {}

				void setBasicData(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR surfaceCapabilities);
				virtual void terminate();

				virtual void initializeSwapChain(UI32 width, UI32 height);
				virtual void terminateSwapChain();

				static VulkanGraphicsSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);

				VkSwapchainKHR swapChain = VK_NULL_HANDLE;
				ARRAY<VkImage> swapChainImages;
				ARRAY<VkImageView> swapChainImageViews;
				VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
				VkExtent2D swapChainExtent = { 0, 0 };

			private:
				void initializeSwapChainImageViews();

				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkSurfaceKHR surface = VK_NULL_HANDLE;
				VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_PIPELINE_H
