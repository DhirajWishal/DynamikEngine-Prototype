#pragma once

namespace Dynamik {
	namespace ADGR {
		namespace core {
			namespace functions {

				struct DMKCreateImageInfo {
					VkDevice device;
					VkPhysicalDevice physicalDevice;

					uint32_t width;
					uint32_t height;

					VkFormat format;
					VkImage* image;
					VkDeviceMemory* imageMemory;

					VkImageTiling tiling;
					VkImageUsageFlags usage;
					VkMemoryPropertyFlags properties;
					VkSampleCountFlagBits numSamples;

					uint32_t mipLevels;
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