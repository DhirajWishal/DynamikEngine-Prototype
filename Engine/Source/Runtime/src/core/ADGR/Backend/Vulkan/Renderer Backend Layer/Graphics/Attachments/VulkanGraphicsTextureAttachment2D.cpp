#include "dmkafx.h"
#include "VulkanGraphicsTextureAttachment2D.h"

#include "../VulkanGraphicsFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanGraphicsTextureAttachment2D::initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos)
			{
				for (const ADGRVulkanTextureInitInfo& info : infos)
				{
					ADGRVulkanTextureContainer _container;
					_container.format = info.format;
					_container.mipLevels = info.mipLevels;

					resource::TextureData texData;
					unsigned char* pixels = nullptr;

					if (_container.format == VK_FORMAT_R8G8B8A8_UNORM)
						pixels = texData.loadTexture((info.path), resource::TEXTURE_TYPE_RGBA);
					else if (_container.format == VK_FORMAT_R8G8B8_UNORM)
						pixels = texData.loadTexture((info.path), resource::TEXTURE_TYPE_RGB);
					else
						DMK_CORE_FATAL("Invalid texture format!");

					_container.width = texData.texWidth;
					_container.height = texData.texHeight;

					VkDeviceSize imageSize = texData.size;

					if (!pixels)
						DMK_CORE_FATAL("failed to load texture image!");

					VkBuffer stagingBuffer = VK_NULL_HANDLE;
					VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

					ADGRVulkanCreateBufferInfo bufferInfo;
					bufferInfo.bufferSize = imageSize;
					bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &stagingBuffer;
					bufferInfo.bufferMemory = &stagingBufferMemory;

					VulkanGraphicsFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);

					void* data;
					if (vkMapMemory(logicalDevice, stagingBufferMemory, 0, static_cast<size_t>(imageSize), 0, &data) != VK_SUCCESS)
						DMK_CORE_FATAL("Failed to map memory!");
					memcpy(data, pixels, static_cast<size_t>(imageSize));
					vkUnmapMemory(logicalDevice, stagingBufferMemory);

					texData.freeData(pixels);

					ADGRVulkanCreateImageInfo cinfo;
					cinfo.width = texData.texWidth;
					cinfo.height = texData.texHeight;
					cinfo.format = _container.format;
					cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
					cinfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
					cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
					cinfo.image = &_container.image;
					cinfo.imageMemory = &_container.imageMemory;
					cinfo.mipLevels = info.mipLevels;
					cinfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
					cinfo.flags = NULL;

					VulkanGraphicsFunctions::createImage(logicalDevice, physicalDevice, cinfo);

					ADGRVulkanTransitionImageLayoutInfo transitionInfo;
					transitionInfo.image = _container.image;
					transitionInfo.format = _container.format;
					transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					transitionInfo.mipLevels = info.mipLevels;
					transitionInfo.layerCount = 1;
					VulkanGraphicsFunctions::transitionImageLayout(logicalDevice, commandPool, processQueue, utilityQueue, transitionInfo);

					ADGRVulkanCopyBufferToImageInfo cpyInfo;
					cpyInfo.buffer = stagingBuffer;
					cpyInfo.image = _container.image;
					cpyInfo.width = static_cast<UI32>(texData.texWidth);
					cpyInfo.height = static_cast<UI32>(texData.texHeight);
					VulkanGraphicsFunctions::copyBufferToImage(logicalDevice, commandPool, processQueue, utilityQueue, cpyInfo);

					transitionInfo.image = _container.image;
					transitionInfo.format = _container.format;
					transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					transitionInfo.mipLevels = info.mipLevels;
					transitionInfo.layerCount = 1;
					VulkanGraphicsFunctions::transitionImageLayout(logicalDevice, commandPool, processQueue, utilityQueue, transitionInfo);

					vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
					vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);

					generateMipMaps(&_container);

					ADGRVulkanTextureSamplerInitInfo samplerInitInfo;
					samplerInitInfo.magFilter = info.magFilter;
					samplerInitInfo.minFilter = info.minFilter;
					samplerInitInfo.maxMipLevels = info.maxMipLevels;
					samplerInitInfo.minMipLevels = info.minMipLevels;
					samplerInitInfo.modeU = info.modeU;
					samplerInitInfo.modeV = info.modeV;
					samplerInitInfo.modeW = info.modeW;
					_container.imageSampler = VulkanGraphicsFunctions::createImageSampler(logicalDevice, samplerInitInfo);

					ADGRVulkanCreateImageViewInfo cinfo2;
					cinfo2.image = _container.image;
					cinfo2.format = _container.format;
					cinfo2.mipLevels = _container.mipLevels;
					cinfo2.aspectFlags = info.aspectFlags;
					_container.imageView = VulkanGraphicsFunctions::createImageView(logicalDevice, cinfo2);

					textureContainers.pushBack(_container);
				}
			}
		}
	}
}