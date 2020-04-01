#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_CORE_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_CORE_OBJECT_H

#include "GLFW/glfw3.h"
#include "DataTypesLib/Public/Array.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanCoreObject {
			public:
				VulkanCoreObject() {}
				~VulkanCoreObject() {}

				VkInstance instance = VK_NULL_HANDLE;
				VkSurfaceKHR surface = VK_NULL_HANDLE;
				VkSurfaceCapabilitiesKHR surfaceCapabilities;

				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

				VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

				VkSwapchainKHR swapChain = VK_NULL_HANDLE;
				ARRAY<VkImage> swapChainImages;
				ARRAY<VkImageView> swapChainImageViews;
				VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
				VkExtent2D swapChainExtent = { 0, 0 };

				VkCommandPool commandPool = VK_NULL_HANDLE;
				ARRAY<VkCommandBuffer> commandBuffers;

				VkRenderPass renderPass = VK_NULL_HANDLE;

				ARRAY<VkFramebuffer> frameBuffers;

				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;

				VkSampleCountFlagBits msaaSamples;

				//VulkanPushConstantManager pushConstant;
				ARRAY<glm::vec4> pushConstants;

				UI32 minMipLevel;
				UI32 maxMipLevel;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_CORE_OBJECT_H
