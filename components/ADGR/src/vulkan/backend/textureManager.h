#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_TEXTURE_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_TEXTURE_MANAGER_H
#ifdef DMK_USE_VULKAN

#include "vulkan/backend/data structures/vulkan.h"
#include "vulkan/backend/functions/textureFunctions.h"
#include "vulkanFormat.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			struct DMKGenerateMipMapInfo {
				VkFormat imageFormat;
				VkImage textureImage;

				I32 width;
				I32 height;

				UI32 mipLevels;

				// for skybox and cubamap
				UI32 size;
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

				UI32 mipLevels = 1;
			};

			struct DMKInitTextureImageViewsInfo {
				VkFormat textureImageFormat = VK_FORMAT_UNDEFINED;
				VkImage textureImage = VK_NULL_HANDLE;

				UI32 mipLevels = 1;
				VkImageAspectFlags flags = 0;
			};

			struct DMKInitTextureSamplerInfo {
				VkSampler* textureSampler = VK_NULL_HANDLE;
				UI32 mipLevel = 1;

				VkFilter magFilter = VK_FILTER_NEAREST;
				VkFilter minFilter = VK_FILTER_NEAREST;

				VkSamplerAddressMode modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				VkSamplerAddressMode modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				VkSamplerAddressMode modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			};

			class textureManager {
			public:
				textureManager() {}
				~textureManager() {}

				void initTexture(ADGRVulkanDataContainer* container, DMKInitTextureInfo info);
				void generateMipMaps(ADGRVulkanDataContainer* container, DMKGenerateMipMapInfo info);
				void initTextureImageViews(ADGRVulkanDataContainer* container, DMKInitTextureImageViewsInfo texInfo, DMKCreateImageViewInfo info);
				void initTextureSampler(ADGRVulkanDataContainer* container, DMKInitTextureSamplerInfo info);

				void clear(ADGRVulkanDataContainer* container, vulkanFormat* format);
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_TEXTURE_H
