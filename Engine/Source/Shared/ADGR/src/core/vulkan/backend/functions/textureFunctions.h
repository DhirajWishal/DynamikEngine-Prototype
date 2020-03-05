#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_TEXTURE_FUNCTIONS_H
#define _DYNAMIK_ADGR_VULKAN_TEXTURE_FUNCTIONS_H
#ifdef DMK_USE_VULKAN

namespace Dynamik {
	namespace ADGR {
		namespace core {
			namespace functions {
				struct DMKCreateImageInfo {
					VkDevice device = VK_NULL_HANDLE;
					VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

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

				struct DMKTransitionImageLayoutInfo {
					VkDevice device;
					VkCommandPool commandPool;

					VkImage image;
					VkFormat format;
					VkImageLayout oldLayout;
					VkImageLayout newLayout;

					VkQueue graphicsQueue;

					UI32 mipLevels;
					UI32 layerCount;

					UI32 arrayLayers = 0;
				};

				struct DMKCreateImageViewInfo {
					VkDevice device;
					VkImage image;
					VkFormat format;
					VkImageAspectFlags aspectFlags;
					VkImageView* textureImageView;

					UI32 mipLevels;
				};

				struct DMKCopyBufferToImageInfo {
					VkDevice device;
					VkCommandPool commandPool;

					VkBuffer buffer;
					VkImage image;

					UI32 width;
					UI32 height;

					UI32 baseArrayCount = 0;
					UI32 layerCount = 1;

					VkQueue graphicsQueue;
				};

				void createImage(DMKCreateImageInfo info);

				void transitionImageLayout(DMKTransitionImageLayoutInfo info);

				VkImageView createImageView(DMKCreateImageViewInfo info);

				void copyBufferToImage(DMKCopyBufferToImageInfo info);
			}
		}
	}
}

#endif
#endif // !_DYNAMIK_ADGR_VULKAN_TEXTURE_FUNCTIONS_H
