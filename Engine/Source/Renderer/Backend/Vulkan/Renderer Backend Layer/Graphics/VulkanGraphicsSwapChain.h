#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_GRAPHICS_SWAP_CHAIN_H
#define _DYNAMIK_RENDERER_VULKAN_GRAPHICS_SWAP_CHAIN_H

#include <vulkan/vulkan.h>
#include "Public/Array.h"
#include "VulkanGraphicsContainers.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanSurfaceContainer;

			struct VulkanGraphicsSwapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities = {};
				ARRAY<VkSurfaceFormatKHR> formats = {};
				ARRAY<VkPresentModeKHR> presentModes = {};
			};

			class VulkanGraphicsSwapChain {
			public:
				VulkanGraphicsSwapChain() {}
				~VulkanGraphicsSwapChain() {}

				void setBasicData(VkDevice device, VkPhysicalDevice physicalDevice, VulkanSurfaceContainer container);

				virtual void initialize(UI32 width, UI32 height, VulkanSurfaceContainer container);
				virtual void terminate();

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
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_PIPELINE_H
