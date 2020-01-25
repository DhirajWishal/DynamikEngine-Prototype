#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_SKYBOX_H
#define _DYNAMIK_ADGR_VULKAN_SKYBOX_H

#include "backend/vulkan/core/data structures/vulkan.h"
#include "texture.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct DMKSkyboxInitInfo {
				std::string path = "";

				VkImage* skyboxImage = nullptr;
				VkImageLayout imageLayout;
				VkSampler imageSampler;

				VkFormat imageFormat;
				VkDeviceMemory* imageMemory = nullptr;

				uint32_t mipLevels;
			};

			struct DMKInitCubemapInfo {
				std::string path = {};

				VkFormat textureImageFormat = VK_FORMAT_UNDEFINED;
				VkImage* textureImage = nullptr;
				VkDeviceMemory* textureImageMemory = VK_NULL_HANDLE;
				VkImageLayout imageLayout;
				VkSampler* imageSampler = nullptr;
				VkImageView* imageView = nullptr;

				uint32_t mipLevels = 1;

				bool flipImage = false;
			};

			struct DMKSkyboxSampelrInitInfo {
				uint32_t mipLevels = 0;

				VkImage skybox = VK_NULL_HANDLE;
				VkFormat format = VK_FORMAT_UNDEFINED;

				VkSampler* sampler = nullptr;
				VkImageView* imageView = nullptr;
			};

			class skyboxManager {
			public:
				skyboxManager() {}
				~skyboxManager() {}

				void initSkybox(ADGRVulkanDataContainer* container, DMKSkyboxInitInfo) ;
				void deleteSkybox(ADGRVulkanDataContainer* container) ;

				void loadCubemap(ADGRVulkanDataContainer* container, DMKInitCubemapInfo initInfo);

				void initSampler(ADGRVulkanDataContainer* container, DMKSkyboxSampelrInitInfo info);

			private:
				void generateMipMaps(ADGRVulkanDataContainer* container, DMKGenerateMipMapInfo info);

				VkImage cubeMap = VK_NULL_HANDLE;
				VkDeviceMemory cubeMapMemory = VK_NULL_HANDLE;

				VkDescriptorSet skyBox = VK_NULL_HANDLE;

				uint32_t width = NULL;
				uint32_t height = NULL;

				uint8_t layerCount = 1;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SKYBOX_H
