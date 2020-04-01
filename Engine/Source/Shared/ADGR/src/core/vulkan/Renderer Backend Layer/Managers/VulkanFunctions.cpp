#include "adgrafx.h"
#include "VulkanFunctions.h"
#include "VulkanOneTimeCommandBufferManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanFunctions::createBuffer(POINTER<VulkanCoreObject> core, ADGRCreateBufferInfo info)
			{
				VkBufferCreateInfo bufferInfo = {};
				bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferInfo.size = info.bufferSize;
				bufferInfo.usage = info.usage;
				bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				if (vkCreateBuffer(core->logicalDevice, &bufferInfo, nullptr, info.buffer) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create buffer!");

				VkMemoryRequirements memRequirements;
				vkGetBufferMemoryRequirements(core->logicalDevice, *info.buffer, &memRequirements);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, info.bufferMemoryPropertyflags,
					core->physicalDevice);

				if (vkAllocateMemory(core->logicalDevice, &allocInfo, nullptr, info.bufferMemory) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate buffer memory!");

				vkBindBufferMemory(core->logicalDevice, *info.buffer, *info.bufferMemory, 0);
			}

			UI32 VulkanFunctions::findMemoryType(UI32 typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice)
			{
				VkPhysicalDeviceMemoryProperties memProperties;
				vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

				for (UI32 i = 0; i < memProperties.memoryTypeCount; i++)
					if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags
						& properties) == properties)
						return i;

				DMK_CORE_FATAL("failed to find suitable memory type!");
				return 0;
			}

			VkSampleCountFlagBits VulkanFunctions::getMaxUsableSampleCount(VkPhysicalDevice physicalDevice)
			{
				VkPhysicalDeviceProperties physicalDeviceProperties;
				vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

				VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts,
					physicalDeviceProperties.limits.framebufferDepthSampleCounts);
				if (counts & VK_SAMPLE_COUNT_64_BIT)  return VK_SAMPLE_COUNT_64_BIT;
				if (counts & VK_SAMPLE_COUNT_32_BIT)  return VK_SAMPLE_COUNT_32_BIT;
				if (counts & VK_SAMPLE_COUNT_16_BIT)  return VK_SAMPLE_COUNT_16_BIT;
				if (counts & VK_SAMPLE_COUNT_8_BIT)  return VK_SAMPLE_COUNT_8_BIT;
				if (counts & VK_SAMPLE_COUNT_4_BIT)  return VK_SAMPLE_COUNT_4_BIT;
				if (counts & VK_SAMPLE_COUNT_2_BIT)  return VK_SAMPLE_COUNT_2_BIT;

				return VK_SAMPLE_COUNT_1_BIT;
			}

			B1 VulkanFunctions::hasStencilComponent(VkFormat format)
			{
				return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
			}

			VkFormat VulkanFunctions::findSupportedFormat(const ARRAY<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice)
			{
				for (VkFormat format : candidates) {
					VkFormatProperties props;

					vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

					if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
						return format;

					else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
						return format;
				}

				DMK_CORE_FATAL("failed to find supported format!");
				return VkFormat();
			}

			VkFormat VulkanFunctions::findDepthFormat(VkPhysicalDevice physicalDevice)
			{
				return findSupportedFormat(
					{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
					VK_IMAGE_TILING_OPTIMAL,
					VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
					physicalDevice
				);
			}

			void VulkanFunctions::copyBuffer(POINTER<VulkanCoreObject> core, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
			{
				VulkanOneTimeCommandBufferManager oneTimeCommandBuffer(*core);
				VkCommandBuffer commandBuffer = oneTimeCommandBuffer.myCommandBuffers[0];
				
				VkBufferCopy copyRegion = {};
				copyRegion.size = size;
				vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
			}

			void VulkanFunctions::createImage(POINTER<VulkanCoreObject> core, ADGRCreateImageInfo info)
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
				imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				imageInfo.usage = info.usage;
				imageInfo.samples = info.numSamples;
				imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				if (info.flags != NULL)
					imageInfo.flags = info.flags;

				if (vkCreateImage(core->logicalDevice, &imageInfo, nullptr, info.image) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create image!");

				VkMemoryRequirements memRequirements;
				vkGetImageMemoryRequirements(core->logicalDevice, *info.image, &memRequirements);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, core->physicalDevice);

				if (vkAllocateMemory(core->logicalDevice, &allocInfo, nullptr, info.imageMemory) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to allocate image memory!");

				vkBindImageMemory(core->logicalDevice, *info.image, *info.imageMemory, 0);
			}

			void VulkanFunctions::transitionImageLayout(POINTER<VulkanCoreObject> core, ADGRTransitionImageLayoutInfo info)
			{
				VulkanOneTimeCommandBufferManager oneTimeCommandBuffer(*core);
				VkCommandBuffer commandBuffer = oneTimeCommandBuffer.myCommandBuffers[0];

				VkImageMemoryBarrier barrier = {};
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.oldLayout = info.oldLayout;
				barrier.newLayout = info.newLayout;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.image = info.image;

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

			VkImageView VulkanFunctions::createImageView(POINTER<VulkanCoreObject> core, ADGRCreateImageViewInfo info)
			{
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
				if (vkCreateImageView(core->logicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create texture image view!");

				return imageView;
			}

			void VulkanFunctions::copyBufferToImage(POINTER<VulkanCoreObject> core, ADGRCopyBufferToImageInfo info)
			{
				VulkanOneTimeCommandBufferManager oneTimeCommandBuffer(*core);
				VkCommandBuffer commandBuffer = oneTimeCommandBuffer.myCommandBuffers[0];

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