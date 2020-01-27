#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_TEXTURE_H
#define _DYNAMIK_ADGR_VULKAN_TEXTURE_H
#ifdef DMK_USE_VULKAN

#include "backend/vulkan/core/data structures/vulkan.h"
#include "backend/vulkan/core/functions/textureFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			struct DMKGenerateMipMapInfo {
				VkFormat imageFormat;
				VkImage textureImage;

				int width;
				int height;

				uint32_t mipLevels;

				// for skybox and cubamap
				uint32_t size;
				VkBuffer* stagingBuffer = nullptr;
				VkImageLayout imageLayout;
				VkImageView* imageView = nullptr;
				VkSampler* imageSampler = nullptr;
			};

			struct DMKInitTextureInfo {
				std::string path = "";

				VkFormat textureImageFormat = VK_FORMAT_UNDEFINED;
				VkImage* textureImage = VK_NULL_HANDLE;
				VkDeviceMemory* textureImageMemory = VK_NULL_HANDLE;

				uint32_t mipLevels = 1;
			};

			struct DMKInitTextureImageViewsInfo {
				VkFormat textureImageFormat = VK_FORMAT_UNDEFINED;
				VkImage textureImage = VK_NULL_HANDLE;

				uint32_t mipLevels = 1;
				VkImageAspectFlags flags = 0;
			};

			struct DMKInitTextureSamplerInfo {
				VkSampler* textureSampler = VK_NULL_HANDLE;
				uint32_t mipLevel = 1;

				VkFilter magFilter = VK_FILTER_NEAREST;
				VkFilter minFilter = VK_FILTER_NEAREST;

				VkSamplerAddressMode modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				VkSamplerAddressMode modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				VkSamplerAddressMode modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			};

			struct DMKTextureDeleteInfo {
				VkSampler sampler = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
				VkImage texture = VK_NULL_HANDLE;
				VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
			};

			class textureManager {
			public:
				textureManager() {}
				~textureManager() {}

				void initTexture(ADGRVulkanDataContainer* container, DMKInitTextureInfo info);
				void generateMipMaps(ADGRVulkanDataContainer* container, DMKGenerateMipMapInfo info);
				void initTextureImageViews(ADGRVulkanDataContainer* container, DMKInitTextureImageViewsInfo texInfo, DMKCreateImageViewInfo info);
				void initTextureSampler(ADGRVulkanDataContainer* container, DMKInitTextureSamplerInfo info);

				void deleteTexture(ADGRVulkanDataContainer* container, DMKTextureDeleteInfo info);
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_TEXTURE_H
