#include "adgrafx.h"
#include "VulkanTexture.h"

#include "VulkanFunctions.h"
#include "VulkanOneTimeCommandBuffer.h"

#include "Platform/Windows/resource/imageLoader.h"
using namespace Dynamik::ADGR::core;

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanTexture::initialize(ADGRVulkanTextureInitInfo info)
			{
				format = info.format;
				mipLevels = info.mipLevels;

				resource::TextureData texData;
				unsigned char* pixels = nullptr;

				if (format == VK_FORMAT_R8G8B8A8_UNORM)
					pixels = texData.loadTexture((info.path), resource::TEXTURE_TYPE_RGBA);
				else if (format == VK_FORMAT_R8G8B8_UNORM)
					pixels = texData.loadTexture((info.path), resource::TEXTURE_TYPE_RGB);
				else
					DMK_CORE_FATAL("Invalid texture format!");

				width = texData.texWidth;
				height = texData.texHeight;

				VkDeviceSize imageSize = texData.size;

				if (!pixels)
					DMK_CORE_FATAL("failed to load texture image!");

				VkBuffer stagingBuffer = VK_NULL_HANDLE;
				VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

				ADGRCreateBufferInfo bufferInfo;
				bufferInfo.device = info.device;
				bufferInfo.bufferSize = imageSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanFunctions::createBuffer(bufferInfo);

				void* data;
				if (vkMapMemory(info.device.logicalDevice, stagingBufferMemory, 0, static_cast<size_t>(imageSize), 0, &data) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to map memory!");
				memcpy(data, pixels, static_cast<size_t>(imageSize));
				vkUnmapMemory(info.device.logicalDevice, stagingBufferMemory);

				texData.freeData(pixels);

				ADGRCreateImageInfo cinfo;
				cinfo.device = info.device;
				cinfo.width = texData.texWidth;
				cinfo.height = texData.texHeight;
				cinfo.format = format;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &image;
				cinfo.imageMemory = &imageMemory;
				cinfo.mipLevels = info.mipLevels;
				cinfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
				cinfo.flags = NULL;

				VulkanFunctions::createImage(cinfo);

				VulkanCommandBuffer _commandBuffer;
				_commandBuffer.myCommandPool = info.commandPool;

				ADGRTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = info.device;
				transitionInfo.commandBuffer = _commandBuffer;
				transitionInfo.image = image;
				transitionInfo.format = format;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.queue = info.queue;
				transitionInfo.mipLevels = info.mipLevels;
				transitionInfo.layerCount = 1;

				VulkanFunctions::transitionImageLayout(transitionInfo);

				ADGRCopyBufferToImageInfo cpyInfo;
				cpyInfo.device = info.device;
				cpyInfo.commandBuffer = _commandBuffer;
				cpyInfo.buffer = stagingBuffer;
				cpyInfo.image = image;
				cpyInfo.width = static_cast<UI32>(texData.texWidth);
				cpyInfo.height = static_cast<UI32>(texData.texHeight);
				cpyInfo.queue = info.queue;

				VulkanFunctions::copyBufferToImage(cpyInfo);

				vkDestroyBuffer(info.device.logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(info.device.logicalDevice, stagingBufferMemory, nullptr);

				generateMipMaps(info.device, info.commandPool, info.queue);

				VkSamplerCreateInfo samplerInfo = {};
				samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				samplerInfo.magFilter = info.magFilter;
				samplerInfo.minFilter = info.minFilter;
				samplerInfo.addressModeU = info.modeU;
				samplerInfo.addressModeV = info.modeV;
				samplerInfo.addressModeW = info.modeW;
				samplerInfo.anisotropyEnable = VK_TRUE;
				samplerInfo.maxAnisotropy = 16;
				samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
				samplerInfo.unnormalizedCoordinates = VK_FALSE;
				samplerInfo.compareEnable = VK_FALSE;
				samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
				samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				samplerInfo.minLod = info.global.minMipLevel; // change this for varying mip levels
				samplerInfo.maxLod = static_cast<F32>(info.global.maxMipLevel);
				samplerInfo.mipLodBias = 0; // Optional

				if (vkCreateSampler(info.device.logicalDevice, &samplerInfo, nullptr, &imageSampler) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create texture sampler!");

				ADGRCreateImageViewInfo cinfo2;
				cinfo2.device = info.device;
				cinfo2.image = image;
				cinfo2.format = format;
				cinfo2.mipLevels = mipLevels;
				cinfo2.aspectFlags = info.aspectFlags;

				imageView = VulkanFunctions::createImageView(cinfo2);
			}

			void VulkanTexture::terminate(VulkanDevice device)
			{
				vkDestroySampler(device.logicalDevice, imageSampler, nullptr);
				vkDestroyImageView(device.logicalDevice, imageView, nullptr);

				vkDestroyImage(device.logicalDevice, image, nullptr);
				vkFreeMemory(device.logicalDevice, imageMemory, nullptr);
			}

			void VulkanTexture::generateMipMaps(VulkanDevice device, VkCommandPool commandPool, VulkanQueue queue)
			{
				UI32 _width = width, _height = height;

				VkFormatProperties formatProperties;
				vkGetPhysicalDeviceFormatProperties(device.physicalDevice, format, &formatProperties);

				if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
					DMK_CORE_FATAL("texture image format does not support linear blitting!");


				VulkanCommandBuffer commandBuffer;
				commandBuffer.myCommandPool = commandPool;

				VulkanOneTimeCommandBuffer oneTimeCommandBuffer(device, commandBuffer, queue);
				VkCommandBuffer _commandBuffer = oneTimeCommandBuffer.myCommandBuffers[0];

				VkImageMemoryBarrier barrier = {};
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.image = image;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				barrier.subresourceRange.baseArrayLayer = 0;
				barrier.subresourceRange.layerCount = 1;
				barrier.subresourceRange.levelCount = 1;

				for (UI32 i = 1; i < mipLevels; i++) {
					barrier.subresourceRange.baseMipLevel = i - 1;
					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

					vkCmdPipelineBarrier(_commandBuffer,
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
						0, nullptr,
						0, nullptr,
						1, &barrier);

					VkImageBlit blit = {};
					blit.srcOffsets[0] = { 0, 0, 0 };
					blit.srcOffsets[1] = { (I32)_width, (I32)_height, 1 };
					blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.srcSubresource.mipLevel = i - 1;
					blit.srcSubresource.baseArrayLayer = 0;
					blit.srcSubresource.layerCount = 1;
					blit.dstOffsets[0] = { 0, 0, 0 };
					blit.dstOffsets[1] = { (I32)_width > 1 ? (I32)_width / 2 : 1, (I32)_height > 1 ? (I32)_height / 2 : 1, 1 };
					blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.dstSubresource.mipLevel = i;
					blit.dstSubresource.baseArrayLayer = 0;
					blit.dstSubresource.layerCount = 1;

					vkCmdBlitImage(_commandBuffer,
						image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						1, &blit,
						VK_FILTER_LINEAR);

					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					vkCmdPipelineBarrier(_commandBuffer,
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
						0, nullptr,
						0, nullptr,
						1, &barrier);

					if (_width > 1) _width /= 2;
					if (_height > 1) _height /= 2;
				}
			}
		}
	}
}