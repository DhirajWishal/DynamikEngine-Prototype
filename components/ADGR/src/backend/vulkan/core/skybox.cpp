#include "adgrafx.h"
#include "skybox.h"

#include "functions/textureFunctions.h"
#include "functions/bufferFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void skybox::initSkybox(ADGRVulkanDataContainer* container, DMKSkyboxInitInfo info) {/*
				DMKInitCubemapInfo cinfo;
				cinfo.path = info.path;
				cinfo.imageLayout = info.imageLayout;
				cinfo.imageSampler = info.imageSampler;
				cinfo.mipLevels = info.mipLevels;
				cinfo.textureImage = info.skyboxImage;
				cinfo.textureImageFormat = info.imageFormat;
				cinfo.textureImageMemory = info.imageMemory;

				loadCubemap(cinfo);*/
			}

			void skybox::deleteSkybox(ADGRVulkanDataContainer* container) {

			}

			void skybox::loadCubemap(ADGRVulkanDataContainer* container, DMKInitCubemapInfo initInfo) {/*
				resource::TextureData texData;

				unsigned char* cubemap = nullptr;

				if (initInfo.textureImageFormat == VK_FORMAT_R8G8B8A8_UNORM)
					cubemap = texData.loadTexture(initInfo.path, resource::TEXTURE_TYPE_RGBA);
				else if (initInfo.textureImageFormat == VK_FORMAT_R8G8B8_UNORM)
					cubemap = texData.loadTexture(initInfo.path, resource::TEXTURE_TYPE_RGB);
				else
					DMK_CORE_FATAL("Invalid texture format!");

				VkDeviceSize imageSize = texData.size;

				width = texData.texWidth;
				height = texData.texHeight;

				if (!cubemap)
					DMK_CORE_FATAL("failed to load texture image!");

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				DMKCreateBufferInfo bufferInfo;
				bufferInfo.device = device;
				bufferInfo.physicalDevice = physicalDevice;
				bufferInfo.bufferSize = imageSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				createBuffer(bufferInfo);

				void* data;
				if (vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to map memory!")
					memcpy(data, cubemap, static_cast<size_t>(imageSize));
				vkUnmapMemory(device, stagingBufferMemory);

				texData.freeData(cubemap);

				mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texData.texWidth,
					texData.texHeight)))) + 1;

				DMKCreateImageInfo info;
				info.device = device;
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

				createImage(info);

				DMKTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = device;
				transitionInfo.commandPool = commandPool;
				transitionInfo.image = *initInfo.textureImage;
				transitionInfo.format = initInfo.textureImageFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.graphicsQueue = graphicsQueue;
				transitionInfo.mipLevels = initInfo.mipLevels;

				transitionImageLayout(transitionInfo);

				DMKCopyBufferToImageInfo cpyInfo;
				cpyInfo.device = device;
				cpyInfo.commandPool = commandPool;
				cpyInfo.buffer = stagingBuffer;
				cpyInfo.image = *initInfo.textureImage;
				cpyInfo.width = static_cast<uint32_t>(texData.texWidth);
				cpyInfo.height = static_cast<uint32_t>(texData.texHeight);
				cpyInfo.graphicsQueue = graphicsQueue;

				copyBufferToImage(cpyInfo);

				vkDestroyBuffer(device, stagingBuffer, nullptr);
				vkFreeMemory(device, stagingBufferMemory, nullptr);

				DMKGenerateMipMapInfo mipmapInfo;
				mipmapInfo.imageFormat = initInfo.textureImageFormat;
				mipmapInfo.textureImage = *initInfo.textureImage;
				mipmapInfo.width = texData.texWidth;
				mipmapInfo.height = texData.texHeight;
				mipmapInfo.mipLevels = initInfo.mipLevels;
				mipmapInfo.imageLayout = initInfo.imageLayout;
				mipmapInfo.size = texData.size;
				mipmapInfo.stagingBuffer = &stagingBuffer;

				generateMipMaps(mipmapInfo);

				vkFreeMemory(device, stagingBufferMemory, nullptr);
				vkDestroyBuffer(device, stagingBuffer, nullptr);*/
			}

			void skybox::generateMipMaps(ADGRVulkanDataContainer* container, DMKGenerateMipMapInfo info) {/*
				VkCommandBuffer commandBuff = beginSingleTimeCommands(device, commandPool);

				std::vector<VkBufferImageCopy> bufferCopyRegions;
				uint32_t offset = 0;

				for (uint32_t face = 0; face < 6; face++) {
					for (uint32_t level = 0; level < info.mipLevels; level++) {
						VkBufferImageCopy bufferCopyRegion = {};
						bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						bufferCopyRegion.imageSubresource.mipLevel = level;
						bufferCopyRegion.imageSubresource.baseArrayLayer = face;
						bufferCopyRegion.imageSubresource.layerCount = 1;
						bufferCopyRegion.imageExtent.width = width;
						bufferCopyRegion.imageExtent.height = height;
						bufferCopyRegion.imageExtent.depth = 1;
						bufferCopyRegion.bufferOffset = offset;

						bufferCopyRegions.push_back(bufferCopyRegion);

						// Increase offset into staging buffer for next level / face
						offset += info.size;
					}
				}

				DMKTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = device;
				transitionInfo.commandPool = commandPool;
				transitionInfo.image = info.textureImage;
				transitionInfo.format = info.imageFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.graphicsQueue = graphicsQueue;
				transitionInfo.mipLevels = info.mipLevels;
				transitionInfo.layerCount = 6;

				transitionImageLayout(transitionInfo);

				// Copy the cube map faces from the staging buffer to the optimal tiled image
				vkCmdCopyBufferToImage(
					commandBuff,
					*info.stagingBuffer,
					info.textureImage,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					static_cast<uint32_t>(bufferCopyRegions.size()),
					bufferCopyRegions.data()
				);

				// Change texture image layout to shader read after all faces have been copied
				info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

				DMKCreateImageViewInfo cinfo;
				cinfo.device = device;
				cinfo.image = info.textureImage;
				cinfo.format = info.imageFormat;
				cinfo.mipLevels = info.mipLevels;
				cinfo.aspectFlags = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

				*info.imageView = createImageView(cinfo);

				info.textureImage;

				DMKSkyboxSampelrInitInfo initInfo = {};
				initInfo.format = info.imageFormat;
				initInfo.imageView = info.imageView;
				initInfo.mipLevels = info.mipLevels;
				initInfo.sampler = &info.imageSampler;
				initInfo.skybox = info.textureImage;

				initSampler(initInfo);

				endSingleTimeCommands(device, commandPool, commandBuff, graphicsQueue);*/
			}

			void skybox::initSampler(ADGRVulkanDataContainer* container, DMKSkyboxSampelrInitInfo info) {/*
				// Create sampler
				VkSamplerCreateInfo sampler = {};
				sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				sampler.magFilter = VK_FILTER_LINEAR;
				sampler.minFilter = VK_FILTER_LINEAR;
				sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				sampler.addressModeV = sampler.addressModeU;
				sampler.addressModeW = sampler.addressModeU;
				sampler.mipLodBias = 0.0f;
				sampler.compareOp = VK_COMPARE_OP_NEVER;
				sampler.minLod = 0.0f;
				sampler.maxLod = info.mipLevels;
				sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
				sampler.maxAnisotropy = 1.0f;
				sampler.anisotropyEnable = VK_TRUE;
				sampler.maxAnisotropy = 16;

				if (vkCreateSampler(device, &sampler, nullptr, info.sampler) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to create Skyboc sampler!");

				// Create image view
				VkImageViewCreateInfo view = {};
				view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				// Cube map view type
				view.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
				view.format = info.format;
				view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
				view.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
				// 6 array layers (faces)
				view.subresourceRange.layerCount = 6;
				// Set number of mip levels
				view.subresourceRange.levelCount = info.mipLevels;
				view.image = info.skybox;
				DMK_CORE_ASSERT((vkCreateImageView(device, &view, nullptr, info.imageView) == VK_SUCCESS),
					"Failed to create Skybox image views!");*/
			}
		}
	}
}
