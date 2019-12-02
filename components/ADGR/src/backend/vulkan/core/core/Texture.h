#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_TEXTURE_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_TEXTURE_H

#include "Core.h"
#include "Platform/Windows/resource/imageLoader.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
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

			struct DMKSkyboxInitInfo {
				std::string path = "";

				VkImage* skyboxImage = nullptr;
				VkImageLayout imageLayout;
				VkSampler imageSampler;

				VkFormat imageFormat;
				VkDeviceMemory* imageMemory = nullptr;

				uint32_t mipLevels;
			};

			class Texture : public Core {
			public:
				Texture() {}
				virtual ~Texture() {}

				virtual void loadTexture(ADGRVulkanDataContainer* container, std::string path, resource::TextureType type) {}
				virtual void loadTexture(std::string path) {}
				virtual void generateMipMaps(ADGRVulkanDataContainer* container, DMKGenerateMipMapInfo info) {}

				virtual void initSkybox(ADGRVulkanDataContainer* container, DMKSkyboxInitInfo info) {}
				virtual void deleteSkybox(ADGRVulkanDataContainer* container) {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDERER_TEXTURE_H
