#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_TEXTURE_FUNCTIONS_H
#define _DYNAMIK_ADGR_VULKAN_TEXTURE_FUNCTIONS_H

namespace Dynamik {
	namespace ADGR {
		namespace core {
			namespace functions {
				struct DMKCreateImageInfo {
					VkDevice device = VK_NULL_HANDLE;
					VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

					uint32_t width = 0;
					uint32_t height = 0;

					VkFormat format = VK_FORMAT_UNDEFINED;
					VkImage* image = nullptr;
					VkDeviceMemory* imageMemory = nullptr;

					VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
					VkImageUsageFlags usage = VK_IMAGE_CREATE_SPARSE_BINDING_BIT;
					VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
					VkSampleCountFlagBits numSamples = VK_SAMPLE_COUNT_1_BIT;

					uint32_t mipLevels = 0;

					uint32_t arrayLayers = 2;

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

					uint32_t mipLevels;
					uint32_t layerCount;

					uint32_t arrayLayers = 0;
				};

				struct DMKCreateImageViewInfo {
					VkDevice device;
					VkImage image;
					VkFormat format;
					VkImageAspectFlags aspectFlags;
					VkImageView* textureImageView;

					uint32_t mipLevels;
				};

				struct DMKCopyBufferToImageInfo {
					VkDevice device;
					VkCommandPool commandPool;

					VkBuffer buffer;
					VkImage image;

					uint32_t width;
					uint32_t height;

					uint32_t baseArrayCount = 0;
					uint32_t layerCount = 1;

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
#endif // !_DYNAMIK_ADGR_VULKAN_TEXTURE_FUNCTIONS_H
