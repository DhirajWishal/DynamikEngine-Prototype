#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_COMPUTE_FUNCTIONS_H
#define _DYNAMIK_RENDERER_VULKAN_COMPUTE_FUNCTIONS_H

#include <vulkan/vulkan.h>
#include "../Common/VulkanUtilities.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			class VulkanComputeFunctions {
				VulkanComputeFunctions() {}
				VulkanComputeFunctions(const VulkanComputeFunctions&) = delete;
				VulkanComputeFunctions(VulkanComputeFunctions&&) = delete;

			public:
				~VulkanComputeFunctions() {}

				static void createImage(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VulkanCreateImageInfo info);
				static void transitionImageLayout(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue computeQueue, VulkanTransitionImageLayoutInfo info);
				static VkImageView createImageView(VkDevice device, VulkanCreateImageViewInfo info);
				static void copyBufferToImage(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue computeQueue, VulkanCopyBufferToImageInfo info);
				static void copyBufferToImageOverride(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue computeQueue, VulkanCopyBufferToImageInfo info, ARRAY<VkBufferImageCopy> copyRegions);
				static VkSampler createImageSampler(VkDevice logicalDevice, VulkanTextureSamplerInitInfo info);
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_COMPUTE_FUNCTIONS_H
