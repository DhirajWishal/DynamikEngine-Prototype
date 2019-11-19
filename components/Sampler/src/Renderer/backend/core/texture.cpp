#include "adgrafx.h"
#include "texture.h"

#include "functions/bufferFunctions.h"

#include "debugger.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void textureManager::generateMipMaps(ADGRVulkanDataContainer* container, DMKGenerateMipMapInfo info) {
				VkFormatProperties formatProperties;
				vkGetPhysicalDeviceFormatProperties(container->physicalDevice, info.imageFormat, &formatProperties);

				if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
					DMK_CORE_FATAL("texture image format does not support linear blitting!");

				VkCommandBuffer commandBuff = beginSingleTimeCommands(container->device, container->commandBufferContainer.commandPool);

				VkImageMemoryBarrier barrier = {};
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.image = info.textureImage;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				barrier.subresourceRange.baseArrayLayer = 0;
				barrier.subresourceRange.layerCount = 1;
				barrier.subresourceRange.levelCount = 1;

				for (uint32_t i = 1; i < info.mipLevels; i++) {
					barrier.subresourceRange.baseMipLevel = i - 1;
					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

					vkCmdPipelineBarrier(commandBuff,
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
						0, nullptr,
						0, nullptr,
						1, &barrier);

					VkImageBlit blit = {};
					blit.srcOffsets[0] = { 0, 0, 0 };
					blit.srcOffsets[1] = { info.width, info.height, 1 };
					blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.srcSubresource.mipLevel = i - 1;
					blit.srcSubresource.baseArrayLayer = 0;
					blit.srcSubresource.layerCount = 1;
					blit.dstOffsets[0] = { 0, 0, 0 };
					blit.dstOffsets[1] = { info.width > 1 ? info.width / 2 : 1, info.height > 1 ? info.height / 2 : 1, 1 };
					blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.dstSubresource.mipLevel = i;
					blit.dstSubresource.baseArrayLayer = 0;
					blit.dstSubresource.layerCount = 1;

					vkCmdBlitImage(commandBuff,
						info.textureImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						info.textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						1, &blit,
						VK_FILTER_LINEAR);

					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					vkCmdPipelineBarrier(commandBuff,
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
						0, nullptr,
						0, nullptr,
						1, &barrier);

					if (info.width > 1) info.width /= 2;
					if (info.height > 1) info.height /= 2;
				}

				barrier.subresourceRange.baseMipLevel = info.mipLevels - 1;
				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				vkCmdPipelineBarrier(commandBuff,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
					0, nullptr,
					0, nullptr,
					1, &barrier);

				endSingleTimeCommands(container->device, container->commandBufferContainer.commandPool, commandBuff, container->graphicsQueue);
			}

			/* TODO: RGB texture loader and SRGB texture loader */
			void textureManager::initTexture(ADGRVulkanDataContainer* container, DMKInitTextureInfo initInfo) {
				resource::TextureData texData;

				unsigned char* pixels = nullptr;

				if (initInfo.textureImageFormat == VK_FORMAT_R8G8B8A8_UNORM)
					pixels = texData.loadTexture(initInfo.path, resource::TEXTURE_TYPE_RGBA);
				else if (initInfo.textureImageFormat == VK_FORMAT_R8G8B8_UNORM)
					pixels = texData.loadTexture(initInfo.path, resource::TEXTURE_TYPE_RGB);
				else
					DMK_CORE_FATAL("Invalid texture format!");

				VkDeviceSize imageSize = texData.size;

				if (!pixels)
					DMK_CORE_FATAL("failed to load texture image!");

				if (!pixels)
					printf("NoData!");

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				DMKCreateBufferInfo bufferInfo;
				bufferInfo.device = container->device;
				bufferInfo.physicalDevice = container->physicalDevice;
				bufferInfo.bufferSize = imageSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				createBuffer(bufferInfo);

				void* data;
				if (vkMapMemory(container->device, stagingBufferMemory, 0, static_cast<size_t>(imageSize), 0, &data) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to map memory!");

				memcpy(data, pixels, static_cast<size_t>(imageSize));
				vkUnmapMemory(container->device, stagingBufferMemory);

				texData.freeData(pixels);

				DMKCreateImageInfo info;
				info.device = container->device;
				info.physicalDevice = container->physicalDevice;
				info.width = texData.texWidth;
				info.height = texData.texHeight;
				info.format = initInfo.textureImageFormat;
				info.tiling = VK_IMAGE_TILING_OPTIMAL;
				info.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				info.image = initInfo.textureImage;
				info.imageMemory = initInfo.textureImageMemory;
				info.mipLevels = initInfo.mipLevels;
				info.numSamples = VK_SAMPLE_COUNT_1_BIT;
				info.flags = NULL;

				createImage(info);

				DMKTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = container->device;
				transitionInfo.commandPool = container->commandBufferContainer.commandPool;
				transitionInfo.image = *initInfo.textureImage;
				transitionInfo.format = initInfo.textureImageFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.graphicsQueue = container->graphicsQueue;
				transitionInfo.mipLevels = initInfo.mipLevels;
				transitionInfo.layerCount = 1;

				transitionImageLayout(transitionInfo);

				DMKCopyBufferToImageInfo cpyInfo;
				cpyInfo.device = container->device;
				cpyInfo.commandPool = container->commandBufferContainer.commandPool;
				cpyInfo.buffer = stagingBuffer;
				cpyInfo.image = *initInfo.textureImage;
				cpyInfo.width = static_cast<uint32_t>(texData.texWidth);
				cpyInfo.height = static_cast<uint32_t>(texData.texHeight);
				cpyInfo.graphicsQueue = container->graphicsQueue;

				copyBufferToImage(cpyInfo);

				vkDestroyBuffer(container->device, stagingBuffer, nullptr);
				vkFreeMemory(container->device, stagingBufferMemory, nullptr);

				DMKGenerateMipMapInfo mipmapInfo;
				mipmapInfo.imageFormat = initInfo.textureImageFormat;
				mipmapInfo.textureImage = *initInfo.textureImage;
				mipmapInfo.width = texData.texWidth;
				mipmapInfo.height = texData.texHeight;
				mipmapInfo.mipLevels = initInfo.mipLevels;

				generateMipMaps(container, mipmapInfo);
			}

			void textureManager::initTextureSampler(ADGRVulkanDataContainer* container, DMKInitTextureSamplerInfo info) {
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
				samplerInfo.minLod = container->minMipLevel; // change this for varying mip levels
				samplerInfo.maxLod = static_cast<float>(container->maxMipLevel);
				samplerInfo.mipLodBias = 0; // Optional

				if (vkCreateSampler(container->device, &samplerInfo, nullptr, info.textureSampler) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create texture sampler!");
			}

			void textureManager::deleteTexture(ADGRVulkanDataContainer* container, DMKTextureDeleteInfo info) {
				vkDestroySampler(container->device, info.sampler, nullptr);
				vkDestroyImageView(container->device, info.imageView, nullptr);

				vkDestroyImage(container->device, info.texture, nullptr);
				vkFreeMemory(container->device, info.textureImageMemory, nullptr);
			}

			void textureManager::initTextureImageViews(ADGRVulkanDataContainer* container, DMKInitTextureImageViewsInfo texInfo, DMKCreateImageViewInfo info) {
				DMKCreateImageViewInfo cinfo;
				cinfo.device = container->device;
				cinfo.image = texInfo.textureImage;
				cinfo.format = texInfo.textureImageFormat;
				cinfo.mipLevels = texInfo.mipLevels;
				cinfo.aspectFlags = texInfo.flags;

				*info.textureImageView = createImageView(cinfo);
			}
		}
	}
}