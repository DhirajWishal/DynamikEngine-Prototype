#include "adgrafx.h"
#ifdef DMK_USE_VULKAN
#include "skybox.h"

#include "functions/textureFunctions.h"
#include "functions/bufferFunctions.h"
#include "Platform/Windows/resource/imageLoader.h"

#include "commandBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void skyboxManager::initSkybox(ADGRVulkanDataContainer* container, DMKSkyboxInitInfo info) {
				DMKInitCubemapInfo cinfo;
				cinfo.path = { info.path };
				cinfo.imageLayout = info.imageLayout;
				cinfo.imageSampler = &info.imageSampler;
				cinfo.mipLevels = info.mipLevels;
				cinfo.textureImage = info.skyboxImage;
				cinfo.textureImageFormat = info.imageFormat;
				cinfo.textureImageMemory = info.imageMemory;

				loadCubemap(container, cinfo);
			}

			void skyboxManager::deleteSkybox(ADGRVulkanDataContainer* container) {
			}

			void skyboxManager::loadCubemap(ADGRVulkanDataContainer* container, DMKInitCubemapInfo initInfo) {
				resource::TextureData texData = {};

				unsigned char* cubemap = texData.loadTexture(initInfo.path, resource::TEXTURE_TYPE_RGBA, initInfo.flipImage);
				VkDeviceSize imageSize = texData.size;

				width = texData.texWidth;
				height = texData.texHeight;

				VkBuffer stagingBuffer = VK_NULL_HANDLE;
				VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

				DMKCreateBufferInfo bufferInfo;
				bufferInfo.device = container->device;
				bufferInfo.physicalDevice = container->physicalDevice;
				bufferInfo.bufferSize = imageSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				createBuffer(bufferInfo);

				void* data = nullptr;
				if (vkMapMemory(container->device, stagingBufferMemory, 0, imageSize, 0, &data) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to map memory!")
					memcpy(data, cubemap, imageSize);
				vkUnmapMemory(container->device, stagingBufferMemory);

				texData.freeData(cubemap);

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
				info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
				info.arrayLayers = layerCount;

				createImage(info);

				DMKGenerateMipMapInfo mipmapInfo;
				mipmapInfo.imageFormat = initInfo.textureImageFormat;
				mipmapInfo.textureImage = *initInfo.textureImage;
				mipmapInfo.width = texData.texWidth;
				mipmapInfo.height = texData.texHeight;
				mipmapInfo.mipLevels = initInfo.mipLevels;
				mipmapInfo.imageLayout = initInfo.imageLayout;
				mipmapInfo.size = texData.size;
				mipmapInfo.stagingBuffer = &stagingBuffer;
				mipmapInfo.imageSampler = initInfo.imageSampler;
				mipmapInfo.imageView = initInfo.imageView;

				generateMipMaps(container, mipmapInfo);

				vkFreeMemory(container->device, stagingBufferMemory, nullptr);
				vkDestroyBuffer(container->device, stagingBuffer, nullptr);
			}

			void skyboxManager::generateMipMaps(ADGRVulkanDataContainer* container, DMKGenerateMipMapInfo info) {
				oneTimeCommandBufferManager oneTimeCommandBuffer(container->device, container->commandBufferContainer.commandPool, container->graphicsQueue);
				VkCommandBuffer commandBuffer = oneTimeCommandBuffer.getCommandBuffers()[0];

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
				transitionInfo.device = container->device;
				transitionInfo.commandPool = container->commandBufferContainer.commandPool;
				transitionInfo.image = info.textureImage;
				transitionInfo.format = info.imageFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.graphicsQueue = container->graphicsQueue;
				transitionInfo.mipLevels = info.mipLevels;
				transitionInfo.layerCount = layerCount;

				transitionImageLayout(transitionInfo);

				DMKCopyBufferToImageInfo cpyInfo;
				cpyInfo.device = container->device;
				cpyInfo.commandPool = container->commandBufferContainer.commandPool;
				cpyInfo.buffer = *info.stagingBuffer;
				cpyInfo.image = info.textureImage;
				cpyInfo.width = static_cast<uint32_t>(width);
				cpyInfo.height = static_cast<uint32_t>(height);
				cpyInfo.graphicsQueue = container->graphicsQueue;

				copyBufferToImage(cpyInfo);

				// Change texture image layout to shader read after all faces have been copied
				info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

				transitionInfo.device = container->device;
				transitionInfo.commandPool = container->commandBufferContainer.commandPool;
				transitionInfo.image = info.textureImage;
				transitionInfo.format = info.imageFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				transitionInfo.newLayout = info.imageLayout;
				transitionInfo.graphicsQueue = container->graphicsQueue;
				transitionInfo.mipLevels = info.mipLevels;
				transitionInfo.layerCount = layerCount;

				transitionImageLayout(transitionInfo);

				// sampler and image view
				DMKSkyboxSampelrInitInfo initInfo = {};
				initInfo.format = info.imageFormat;
				initInfo.imageView = info.imageView;
				initInfo.mipLevels = info.mipLevels;
				initInfo.sampler = info.imageSampler;
				initInfo.skybox = info.textureImage;

				initSampler(container, initInfo);
			}

			void skyboxManager::initSampler(ADGRVulkanDataContainer* container, DMKSkyboxSampelrInitInfo info) {
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

				if (vkCreateSampler(container->device, &sampler, nullptr, info.sampler) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to create Skybox sampler!");

				// Create image view
				VkImageViewCreateInfo view = {};
				view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				// Cube map view type
				view.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
				view.format = info.format;
				view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
				view.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
				// 6 array layers (faces)
				view.subresourceRange.layerCount = layerCount;
				// Set number of mip levels
				view.subresourceRange.levelCount = info.mipLevels;
				view.image = info.skybox;
				DMK_CORE_ASSERT((vkCreateImageView(container->device, &view, nullptr, info.imageView) == VK_SUCCESS),
					"Failed to create Skybox image views!");
			}
		}
	}
}
#endif // DMK_USE_VULKAN
