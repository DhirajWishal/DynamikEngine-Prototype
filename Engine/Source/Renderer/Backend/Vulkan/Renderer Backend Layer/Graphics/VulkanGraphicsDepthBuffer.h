#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_GRAPHICS_DEPTH_BUFFER_H
#define _DYNAMIK_RENDERER_VULKAN_GRAPHICS_DEPTH_BUFFER_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			class VulkanGraphicsDepthBuffer {
			public:
				VulkanGraphicsDepthBuffer() {}
				~VulkanGraphicsDepthBuffer() {}

				void initialize(
					VkDevice logicalDevice,
					VkPhysicalDevice physicalDevice,
					VkCommandPool commandPool,
					VkQueue graphicsQueue,
					VkQueue presentQueue,
					VkExtent2D swapChainExtent,
					VkSampleCountFlagBits msaaSamples);

				void terminate(VkDevice logicalDevice);

				VkImage image = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_DEPTH_BUFFER_H
