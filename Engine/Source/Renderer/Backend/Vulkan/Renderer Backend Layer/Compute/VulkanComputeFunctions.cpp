#include "dmkafx.h"
#include "VulkanComputeFunctions.h"

#include "../Common/VulkanOneTimeCommandBuffer.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			void VulkanComputeFunctions::createImage(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VulkanCreateImageInfo info)
			{
				VkImageCreateInfo imageInfo = {};
				imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				imageInfo.imageType = VK_IMAGE_TYPE_2D;
				imageInfo.extent.width = info.width;
				imageInfo.extent.height = info.height;
				imageInfo.extent.depth = 1;
				imageInfo.mipLevels = info.mipLevels;
				imageInfo.arrayLayers = info.arrayLayers;
				imageInfo.format = info.format;
				imageInfo.initialLayout = info.initialLayout;
				imageInfo.usage = info.usage;
				imageInfo.samples = info.numSamples;
				imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				if (info.flags != NULL)
					imageInfo.flags = info.flags;

				if (vkCreateImage(logicalDevice, &imageInfo, nullptr, info.image) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create image!");

				VkMemoryRequirements memRequirements;
				vkGetImageMemoryRequirements(logicalDevice, *info.image, &memRequirements);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = VulkanUtilities::findMemoryType(memRequirements.memoryTypeBits,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, physicalDevice);

				if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, info.imageMemory) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate image memory!");

				vkBindImageMemory(logicalDevice, *info.image, *info.imageMemory, 0);
			}
			
			void VulkanComputeFunctions::transitionImageLayout(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue computeQueue, VulkanTransitionImageLayoutInfo info)
			{
				//VulkanOneTimeCommandBuffer oneTimeCommandBuffer(logicalDevice, commandPool, computeQueue);

				VkImageMemoryBarrier barrier = {};
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.oldLayout = info.oldLayout;
				barrier.newLayout = info.newLayout;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.image = info.image;

				if (info.newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
					barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

					if (VulkanUtilities::hasStencilComponent(info.format)) {
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

				switch (info.oldLayout)
				{
				case VK_IMAGE_LAYOUT_UNDEFINED:
					sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
					barrier.srcAccessMask = 0;
					break;

				case VK_IMAGE_LAYOUT_PREINITIALIZED:
					barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
					barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
					barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					break;

				case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
					sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
					destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
					barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
					break;
				default:
					DMK_CORE_FATAL("unsupported layout transition!");
					break;
				}

				switch (info.newLayout)
				{
				case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
					destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					break;

				case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
					destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
					destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
					barrier.dstAccessMask = barrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
					break;

				case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
					if (barrier.srcAccessMask == 0)
					{
						barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
					}
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
					destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
					break;

				case VK_IMAGE_LAYOUT_GENERAL:
					destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
					break;
				default:
					DMK_CORE_FATAL("unsupported layout transition!");
					break;
				}

				//vkCmdPipelineBarrier(
				//	oneTimeCommandBuffer.buffer,
				//	sourceStage, destinationStage,
				//	0,
				//	0, nullptr,
				//	0, nullptr,
				//	1, &barrier
				//	);
			}
			
			VkImageView VulkanComputeFunctions::createImageView(VkDevice device, VulkanCreateImageViewInfo info)
			{
				VkImageViewCreateInfo viewInfo = {};
				viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				viewInfo.image = info.image;
				viewInfo.viewType = info.viewType;
				viewInfo.format = info.format;
				viewInfo.subresourceRange.aspectMask = info.aspectFlags;
				viewInfo.subresourceRange.baseMipLevel = 0;
				viewInfo.subresourceRange.levelCount = info.mipLevels;
				viewInfo.subresourceRange.baseArrayLayer = 0;
				viewInfo.subresourceRange.layerCount = info.layerCount;
				viewInfo.components = info.component;

				VkImageView imageView;
				if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create texture image view!");

				return imageView;
			}
			
			void VulkanComputeFunctions::copyBufferToImage(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue computeQueue, VulkanCopyBufferToImageInfo info)
			{
				//VulkanComputeOneTimeCommandBuffer oneTimeCommandBuffer(logicalDevice, commandPool, computeQueue);

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

				//vkCmdCopyBufferToImage(
				//	oneTimeCommandBuffer.buffer,
				//	info.buffer,
				//	info.image,
				//	info.destinationImageLayout,
				//	1,
				//	&region
				//	);
			}
			
			void VulkanComputeFunctions::copyBufferToImageOverride(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue computeQueue, VulkanCopyBufferToImageInfo info, ARRAY<VkBufferImageCopy> copyRegions)
			{
				//VulkanComputeOneTimeCommandBuffer oneTimeCommandBuffer(logicalDevice, commandPool, computeQueue);
				//
				//vkCmdCopyBufferToImage(
				//	oneTimeCommandBuffer.buffer,
				//	info.buffer,
				//	info.image,
				//	info.destinationImageLayout,
				//	copyRegions.size(),
				//	copyRegions.data());
			}
			
			VkSampler VulkanComputeFunctions::createImageSampler(VkDevice logicalDevice, VulkanTextureSamplerInitInfo info)
			{
				VkSamplerCreateInfo samplerInfo = {};
				samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				samplerInfo.magFilter = info.magFilter;
				samplerInfo.minFilter = info.minFilter;
				samplerInfo.addressModeU = info.modeU;
				samplerInfo.addressModeV = info.modeV;
				samplerInfo.addressModeW = info.modeW;
				samplerInfo.anisotropyEnable = info.anisotrophyEnable;
				samplerInfo.maxAnisotropy = info.maxAnisotrophy;
				samplerInfo.borderColor = info.borderColor;
				samplerInfo.unnormalizedCoordinates = info.unnormalizedCoordinates;
				samplerInfo.compareEnable = info.compareEnable;
				samplerInfo.compareOp = info.compareOp;
				samplerInfo.mipmapMode = info.mipMapMode;
				samplerInfo.minLod = info.minMipLevels; // change this for varying mip levels
				samplerInfo.maxLod = static_cast<F32>(info.maxMipLevels);
				samplerInfo.mipLodBias = info.mipLoadBias; // Optional

				VkSampler _sampler = VK_NULL_HANDLE;
				if (vkCreateSampler(logicalDevice, &samplerInfo, nullptr, &_sampler) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create texture sampler!");

				return _sampler;
			}
		}
	}
}