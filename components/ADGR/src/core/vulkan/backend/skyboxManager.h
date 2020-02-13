#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_SKYBOX_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_SKYBOX_MANAGER_H
#ifdef DMK_USE_VULKAN

#include "core/vulkan/backend/data structures/vulkan.h"
#include "textureManager.h"

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

				UI32 mipLevels;
			};

			struct DMKInitCubemapInfo {
				std::string path = {};

				VkFormat textureImageFormat = VK_FORMAT_UNDEFINED;
				VkImage* textureImage = nullptr;
				VkDeviceMemory* textureImageMemory = VK_NULL_HANDLE;
				VkImageLayout imageLayout;
				VkSampler* imageSampler = nullptr;
				VkImageView* imageView = nullptr;

				UI32 mipLevels = 1;

				B1 flipImage = false;
			};

			struct DMKSkyboxSampelrInitInfo {
				UI32 mipLevels = 0;

				VkImage skybox = VK_NULL_HANDLE;
				VkFormat format = VK_FORMAT_UNDEFINED;

				VkSampler* sampler = nullptr;
				VkImageView* imageView = nullptr;
			};

			class skyboxManager {
			public:
				skyboxManager() {}
				~skyboxManager() {}

				void initSkybox(ADGRVulkanDataContainer* container, DMKSkyboxInitInfo);
				void deleteSkybox(ADGRVulkanDataContainer* container);

				void loadCubemap(ADGRVulkanDataContainer* container, DMKInitCubemapInfo initInfo);

				void initSampler(ADGRVulkanDataContainer* container, DMKSkyboxSampelrInitInfo info);

			private:
				void generateMipMaps(ADGRVulkanDataContainer* container, DMKGenerateMipMapInfo info);

				VkImage cubeMap = VK_NULL_HANDLE;
				VkDeviceMemory cubeMapMemory = VK_NULL_HANDLE;

				VkDescriptorSet skyBox = VK_NULL_HANDLE;

				UI32 width = NULL;
				UI32 height = NULL;

				uint8_t layerCount = 1;
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_SKYBOX_H
