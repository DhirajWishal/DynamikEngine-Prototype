#include "adgrafx.h"
#if defined(DMK_USE_VULKAN)

#include "textureFunctions.h"

#include "bufferFunctions.h"

#include "defines.h"
#include "core/vulkan/backend/commandBufferManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			namespace functions {
				void createImage(DMKCreateImageInfo info) {
					VkImageCreateInfo imageInfo = {};
					imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
					imageInfo.imageType = VK_IMAGE_TYPE_2D;
					imageInfo.extent.width = info.width;
					imageInfo.extent.height = info.height;
					imageInfo.extent.depth = 1;
					imageInfo.mipLevels = info.mipLevels;
					imageInfo.arrayLayers = info.arrayLayers;
					imageInfo.format = info.format;
					imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					imageInfo.usage = info.usage;
					imageInfo.samples = info.numSamples;
					imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

					if (info.flags != NULL)
						imageInfo.flags = info.flags;

					if (vkCreateImage(info.device, &imageInfo, nullptr, info.image) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create image!");

					VkMemoryRequirements memRequirements;
					vkGetImageMemoryRequirements(info.device, *info.image, &memRequirements);

					VkMemoryAllocateInfo allocInfo = {};
					allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
					allocInfo.allocationSize = memRequirements.size;
					allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
						VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, info.physicalDevice);

					if (vkAllocateMemory(info.device, &allocInfo, nullptr, info.imageMemory) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to allocate image memory!");

					vkBindImageMemory(info.device, *info.image, *info.imageMemory, 0);
				}

				VkImageView createImageView(DMKCreateImageViewInfo info) {
					VkImageViewCreateInfo viewInfo = {};
					viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
					viewInfo.image = info.image;
					viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
					viewInfo.format = info.format;
					viewInfo.subresourceRange.aspectMask = info.aspectFlags;
					viewInfo.subresourceRange.baseMipLevel = 0;
					viewInfo.subresourceRange.levelCount = info.mipLevels;
					viewInfo.subresourceRange.baseArrayLayer = 0;
					viewInfo.subresourceRange.layerCount = 1;

					VkImageView imageView;
					if (vkCreateImageView(info.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create texture image view!");

					return imageView;
				}

				void transitionImageLayout(DMKTransitionImageLayoutInfo info) {
					oneTimeCommandBufferManager oneTimeCommandBuffer(info.device, info.commandPool, info.graphicsQueue);
					VkCommandBuffer commandBuffer = oneTimeCommandBuffer.getCommandBuffers()[0];

					VkImageMemoryBarrier barrier = {};
					barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
					barrier.oldLayout = info.oldLayout;
					barrier.newLayout = info.newLayout;
					barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
					barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
					barrier.image = info.image;
					//barrier.subresourceRange.levelCount = mipLevels;

					if (info.newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
						barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

						if (hasStencilComponent(info.format)) {
							barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
						}
					}
					else {
						barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					}

					barrier.subresourceRange.baseMipLevel = 0;
					barrier.subresourceRange.levelCount = info.mipLevels;
					barrier.subresourceRange.baseArrayLayer = info.arrayLayers;
					barrier.subresourceRange.layerCount = info.layerCount;
					barrier.srcAccessMask = 0; // TODO
					barrier.dstAccessMask = 0; // TODO

					VkPipelineStageFlags sourceStage{};
					VkPipelineStageFlags destinationStage{};

					if (info.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && info.newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
						barrier.srcAccessMask = 0;
						barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

						sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
						destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
					}
					else if (info.oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && info.newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
						barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
						barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

						sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
						destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
					}
					else if (info.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && info.newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
						barrier.srcAccessMask = 0;
						barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

						sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
						destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
					}
					else if (info.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && info.newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
						barrier.srcAccessMask = 0;
						barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
						sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
						destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					}
					else {
						DMK_CORE_FATAL("unsupported layout transition!");
					}

					vkCmdPipelineBarrier(
						commandBuffer,
						sourceStage, destinationStage,
						0,
						0, nullptr,
						0, nullptr,
						1, &barrier
					);
				}

				void copyBufferToImage(DMKCopyBufferToImageInfo info) {
					oneTimeCommandBufferManager oneTimeCommandBuffer(info.device, info.commandPool, info.graphicsQueue);
					VkCommandBuffer commandBuffer = oneTimeCommandBuffer.getCommandBuffers()[0];

					VkBufferImageCopy region = {};
					region.bufferOffset = 0;
					region.bufferRowLength = 0;
					region.bufferImageHeight = 0;

					region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					region.imageSubresource.mipLevel = 0;
					region.imageSubresource.baseArrayLayer = info.baseArrayCount;
					region.imageSubresource.layerCount = info.layerCount;

					region.imageOffset = { 0, 0, 0 };
					region.imageExtent = {
						info.width,
						info.height,
						1
					};

					vkCmdCopyBufferToImage(
						commandBuffer,
						info.buffer,
						info.image,
						VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						1,
						&region
					);
				}
			}
		}
	}
}

#endif