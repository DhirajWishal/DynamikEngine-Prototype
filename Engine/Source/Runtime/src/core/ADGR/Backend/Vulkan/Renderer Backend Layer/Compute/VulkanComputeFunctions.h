#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_FUNCTIONS_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_FUNCTIONS_H

#include <vulkan/vulkan.h>
#include "../Graphics/VulkanGraphicsFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanComputeFunctions {
				VulkanComputeFunctions() {}
				VulkanComputeFunctions(const VulkanComputeFunctions&) = delete;
				VulkanComputeFunctions(VulkanComputeFunctions&&) = delete;

			public:
				~VulkanComputeFunctions() {}

				static void createImage(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, ADGRVulkanCreateImageInfo info);
				static void transitionImageLayout(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue computeQueue, ADGRVulkanTransitionImageLayoutInfo info);
				static VkImageView createImageView(VkDevice device, ADGRVulkanCreateImageViewInfo info);
				static void copyBufferToImage(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue computeQueue, ADGRVulkanCopyBufferToImageInfo info);
				static void copyBufferToImageOverride(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue computeQueue, ADGRVulkanCopyBufferToImageInfo info, ARRAY<VkBufferImageCopy> copyRegions);
				static VkSampler createImageSampler(VkDevice logicalDevice, ADGRVulkanTextureSamplerInitInfo info);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_FUNCTIONS_H
