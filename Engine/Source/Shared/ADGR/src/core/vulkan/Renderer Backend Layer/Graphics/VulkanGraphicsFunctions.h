#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_FUNCTIONS_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_FUNCTIONS_H

#include "VulkanGraphicsRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanCreateBufferInfo {
				VkBuffer* buffer = nullptr;
				VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				VkDeviceSize bufferSize = 0;
				VkDeviceMemory* bufferMemory = nullptr;

				VkMemoryPropertyFlags bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			};

			struct ADGRVulkanCreateImageInfo {
				UI32 width = 0;
				UI32 height = 0;

				VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

				VkFormat format = VK_FORMAT_UNDEFINED;
				VkImage* image = nullptr;
				VkDeviceMemory* imageMemory = nullptr;

				VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
				VkImageUsageFlags usage = VK_IMAGE_CREATE_SPARSE_BINDING_BIT;
				VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				VkSampleCountFlagBits numSamples = VK_SAMPLE_COUNT_1_BIT;

				UI32 mipLevels = 0;

				UI32 arrayLayers = 2;

				VkImageCreateFlags flags = VK_IMAGE_CREATE_SPARSE_BINDING_BIT;
			};

			struct ADGRVulkanTransitionImageLayoutInfo {
				VkImage image;
				VkFormat format;
				VkImageLayout oldLayout;
				VkImageLayout newLayout;

				UI32 mipLevels;
				UI32 layerCount;

				UI32 arrayLayers = 0;
			};

			struct ADGRVulkanCreateImageViewInfo {
				VkImage image;
				VkFormat format;
				VkImageAspectFlags aspectFlags;
				VkImageView* textureImageView;

				VkComponentMapping component = {};

				VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D;

				UI32 mipLevels;
				UI32 layerCount = 1;
			};

			struct ADGRVulkanCopyBufferToImageInfo {
				VkBuffer buffer;
				VkImage image;
				VkImageLayout destinationImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

				UI32 width;
				UI32 height;

				UI32 baseArrayCount = 0;
				UI32 layerCount = 1;
			};

			struct ADGRVulkanTextureSamplerInitInfo {
				VkFilter magFilter;
				VkFilter minFilter;
				VkSamplerAddressMode modeU;
				VkSamplerAddressMode modeV;
				VkSamplerAddressMode modeW;

				F32 minMipLevels;
				F32 maxMipLevels;

				VkBool32 anisotrophyEnable = VK_FALSE;
				F32 maxAnisotrophy = 16;
				VkBorderColor borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
				VkBool32 unnormalizedCoordinates = VK_FALSE;
				VkBool32 compareEnable = VK_FALSE;
				VkCompareOp compareOp = VK_COMPARE_OP_ALWAYS;
				VkSamplerMipmapMode mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				F32 mipLoadBias = 0;
			};

			class VulkanGraphicsFunctions {
			public:
				VulkanGraphicsFunctions() {}
				~VulkanGraphicsFunctions() {}

				/* BUFFER FUNCTIONS */
				static void createBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, ADGRVulkanCreateBufferInfo info);
				static UI32 findMemoryType(UI32 typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);
				static VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice);
				static B1 hasStencilComponent(VkFormat format);
				static VkFormat findSupportedFormat(const ARRAY<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice);
				static VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
				static void copyBuffer(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkQueue presentQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

				/* TEXTURE FUNCTIONS */
				static void createImage(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, ADGRVulkanCreateImageInfo info);
				static void transitionImageLayout(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkQueue presentQueue, ADGRVulkanTransitionImageLayoutInfo info);
				static VkImageView createImageView(VkDevice device, ADGRVulkanCreateImageViewInfo info);
				static void copyBufferToImage(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkQueue presentQueue, ADGRVulkanCopyBufferToImageInfo info);
				static void copyBufferToImageOverride(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkQueue presentQueue, ADGRVulkanCopyBufferToImageInfo info, ARRAY<VkBufferImageCopy> copyRegions);
				static VkSampler createImageSampler(VkDevice logicalDevice, ADGRVulkanTextureSamplerInitInfo info);

				static ADGRVulkanUnformBufferContainer createUniformBuffers(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize, UI32 count);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_BUFFER_FUNCTIONS_H
