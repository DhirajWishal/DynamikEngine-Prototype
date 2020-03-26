#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_FUNCTIONS_H
#define _DYNAMIK_ADGR_VULKAN_FUNCTIONS_H

#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"
#include "VulkanQueue.h"
#include "VulkanGlobalVariables.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRCreateBufferInfo {
				VulkanDevice device;

				VkBuffer* buffer = nullptr;
				VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				VkDeviceSize bufferSize = 0;
				VkDeviceMemory* bufferMemory = nullptr;

				VkMemoryPropertyFlags bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			};

			struct ADGRCreateImageInfo {
				VulkanDevice device;

				UI32 width = 0;
				UI32 height = 0;

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

			struct ADGRTransitionImageLayoutInfo {
				VulkanDevice device;
				VulkanCommandBuffer commandBuffer;

				VkImage image;
				VkFormat format;
				VkImageLayout oldLayout;
				VkImageLayout newLayout;

				VulkanQueue queue;

				UI32 mipLevels;
				UI32 layerCount;

				UI32 arrayLayers = 0;
			};

			struct ADGRCreateImageViewInfo {
				VulkanDevice device;

				VkImage image;
				VkFormat format;
				VkImageAspectFlags aspectFlags;
				VkImageView* textureImageView;

				UI32 mipLevels;
			};

			struct ADGRCopyBufferToImageInfo {
				VulkanDevice device;
				VulkanCommandBuffer commandBuffer;
				VulkanQueue queue;

				VkBuffer buffer;
				VkImage image;

				UI32 width;
				UI32 height;

				UI32 baseArrayCount = 0;
				UI32 layerCount = 1;
			};

			class VulkanFunctions {
			public:
				VulkanFunctions() {}
				~VulkanFunctions() {}

				/* BUFFER FUNCTIONS */
				static void createBuffer(ADGRCreateBufferInfo info);
				static UI32 findMemoryType(UI32 typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);
				static VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice);
				static B1 hasStencilComponent(VkFormat format);
				static VkFormat findSupportedFormat(const ARRAY<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice);
				static VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
				static void copyBuffer(VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
					VkCommandPool commandPool, VkQueue graphicsQueue);

				/* TEXTURE FUNCTIONS */
				static void createImage(ADGRCreateImageInfo info);
				static void transitionImageLayout(ADGRTransitionImageLayoutInfo info);
				static VkImageView createImageView(ADGRCreateImageViewInfo info);
				static void copyBufferToImage(ADGRCopyBufferToImageInfo info);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_BUFFER_FUNCTIONS_H
