#include "adgrafx.h"
#include "texture.h"

#include "functions/bufferFunctions.h"

#include "debugger.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void textureManager::loadTexture(std::string path, resource::TextureType type) {
				resource::TextureData texData;
				unsigned char* pixels = texData.loadTexture(path, type);

				VkDeviceSize imageSize = texData.size;
				*m_mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texData.texWidth, texData.texHeight)))) + 1;

				if (!pixels)
					DMK_CORE_FATAL("failed to load texture image!");

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				DMKCreateBufferInfo bufferInfo;
				bufferInfo.device = *m_device;
				bufferInfo.physicalDevice = physicalDevice;
				bufferInfo.bufferSize = imageSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				createBuffer(bufferInfo);

				void* data;
				vkMapMemory(*m_device, stagingBufferMemory, 0, imageSize, 0, &data);
				memcpy(data, pixels, static_cast<size_t>(imageSize));
				vkUnmapMemory(*m_device, stagingBufferMemory);

				texData.freeData(pixels);

				DMKCreateImageInfo info;
				info.device = *m_device;
				info.physicalDevice = physicalDevice;
				info.width = texData.texWidth;
				info.height = texData.texHeight;
				info.format = VK_FORMAT_R8G8B8A8_UNORM;
				info.tiling = VK_IMAGE_TILING_OPTIMAL;
				info.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				info.image = m_textureImage;
				info.imageMemory = m_textureImageMemory;
				info.mipLevels = mipLevels;
				info.numSamples = VK_SAMPLE_COUNT_1_BIT;

				createImage(info);

				DMKTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = *m_device;
				transitionInfo.commandPool = commandPool;
				transitionInfo.image = *m_textureImage;
				transitionInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.graphicsQueue = graphicsQueue;
				transitionInfo.mipLevels = *m_mipLevels;

				transitionImageLayout(transitionInfo);

				DMKCopyBufferToImageInfo cpyInfo;
				cpyInfo.device = *m_device;
				cpyInfo.commandPool = commandPool;
				cpyInfo.buffer = stagingBuffer;
				cpyInfo.image = *m_textureImage;
				cpyInfo.width = static_cast<uint32_t>(texData.texWidth);
				cpyInfo.height = static_cast<uint32_t>(texData.texHeight);
				cpyInfo.graphicsQueue = graphicsQueue;

				copyBufferToImage(cpyInfo);

				vkDestroyBuffer(*m_device, stagingBuffer, nullptr);
				vkFreeMemory(*m_device, stagingBufferMemory, nullptr);

				DMKGenerateMipMapInfo mipmapInfo;
				mipmapInfo.imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
				mipmapInfo.textureImage = *m_textureImage;
				mipmapInfo.width = texData.texWidth;
				mipmapInfo.height = texData.texHeight;
				mipmapInfo.mipLevels = *m_mipLevels;

				generateMipMaps(mipmapInfo);
			}

			void textureManager::generateMipMaps(DMKGenerateMipMapInfo info) {
				VkFormatProperties formatProperties;
				vkGetPhysicalDeviceFormatProperties(physicalDevice, info.imageFormat, &formatProperties);

				if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
					DMK_CORE_FATAL("texture image format does not support linear blitting!");

				VkCommandBuffer commandBuff = beginSingleTimeCommands(*m_device, commandPool);

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

				endSingleTimeCommands(*m_device, commandPool, commandBuff, graphicsQueue);
			}

			/* TODO: RGB texture loader and SRGB texture loader */
			void textureManager::initTexture(DMKInitTextureInfo initInfo) {
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

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				DMKCreateBufferInfo bufferInfo;
				bufferInfo.device = *m_device;
				bufferInfo.physicalDevice = physicalDevice;
				bufferInfo.bufferSize = imageSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				createBuffer(bufferInfo);

				void* data;
				if(vkMapMemory(*m_device, stagingBufferMemory, 0, imageSize, 0, &data) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to map memory!")
				memcpy(data, pixels, static_cast<size_t>(imageSize));
				vkUnmapMemory(*m_device, stagingBufferMemory);

				texData.freeData(pixels);

				mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texData.texWidth,
					texData.texHeight)))) + 1;

				DMKCreateImageInfo info;
				info.device = *m_device;
				info.physicalDevice = physicalDevice;
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
				transitionInfo.device = *m_device;
				transitionInfo.commandPool = commandPool;
				transitionInfo.image = *initInfo.textureImage;
				transitionInfo.format = initInfo.textureImageFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.graphicsQueue = graphicsQueue;
				transitionInfo.mipLevels = initInfo.mipLevels;
				transitionInfo.layerCount = 1;

				transitionImageLayout(transitionInfo);

				DMKCopyBufferToImageInfo cpyInfo;
				cpyInfo.device = *m_device;
				cpyInfo.commandPool = commandPool;
				cpyInfo.buffer = stagingBuffer;
				cpyInfo.image = *initInfo.textureImage;
				cpyInfo.width = static_cast<uint32_t>(texData.texWidth);
				cpyInfo.height = static_cast<uint32_t>(texData.texHeight);
				cpyInfo.graphicsQueue = graphicsQueue;

				copyBufferToImage(cpyInfo);

				vkDestroyBuffer(*m_device, stagingBuffer, nullptr);
				vkFreeMemory(*m_device, stagingBufferMemory, nullptr);

				DMKGenerateMipMapInfo mipmapInfo;
				mipmapInfo.imageFormat = initInfo.textureImageFormat;
				mipmapInfo.textureImage = *initInfo.textureImage;
				mipmapInfo.width = texData.texWidth;
				mipmapInfo.height = texData.texHeight;
				mipmapInfo.mipLevels = initInfo.mipLevels;

				generateMipMaps(mipmapInfo);
			}

			void textureManager::initTextureImageView() {
				DMKCreateImageViewInfo info;
				info.device = *m_device;
				info.image = *m_textureImage;
				info.format = VK_FORMAT_R8G8B8A8_UNORM;
				info.mipLevels = *m_mipLevels;
				info.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

				*m_textureImageView = createImageView(info);
			}

			void textureManager::initTextureSampler() {
				VkSamplerCreateInfo samplerInfo = {};
				samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				samplerInfo.magFilter = VK_FILTER_LINEAR;
				samplerInfo.minFilter = VK_FILTER_LINEAR;
				samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				samplerInfo.anisotropyEnable = VK_TRUE;
				samplerInfo.maxAnisotropy = 16;
				samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
				samplerInfo.unnormalizedCoordinates = VK_FALSE;
				samplerInfo.compareEnable = VK_FALSE;
				samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
				samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				samplerInfo.minLod = *minMip; // change this for varing mip levels
				samplerInfo.maxLod = static_cast<float>(*maxMip);
				samplerInfo.mipLodBias = 0; // Optional

				if (vkCreateSampler(*m_device, &samplerInfo, nullptr, m_textureSampler) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create texture sampler!");
			}

			void textureManager::initTextureSampler(DMKInitTextureSamplerInfo info) {
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
				samplerInfo.minLod = *minMip; // change this for varying mip levels
				samplerInfo.maxLod = static_cast<float>(*maxMip);
				samplerInfo.mipLodBias = 0; // Optional

				if (vkCreateSampler(*m_device, &samplerInfo, nullptr, info.textureSampler) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create texture sampler!");
			}

			void textureManager::deleteTexture(DMKTextureDeleteInfo info) {
				vkDestroySampler(*m_device, info.sampler, nullptr);
				vkDestroyImageView(*m_device, info.imageView, nullptr);

				vkDestroyImage(*m_device, info.texture, nullptr);
				vkFreeMemory(*m_device, info.textureImageMemory, nullptr);
			}

			void textureManager::initTextureImageViews(DMKInitTextureImageViewsInfo texInfo, DMKCreateImageViewInfo info) {
				DMKCreateImageViewInfo cinfo;
				cinfo.device = *m_device;
				cinfo.image = texInfo.textureImage;
				cinfo.format = texInfo.textureImageFormat;
				cinfo.mipLevels = texInfo.mipLevels;
				cinfo.aspectFlags = texInfo.flags;

				*info.textureImageView = createImageView(cinfo);
			}
		}
	}
}
