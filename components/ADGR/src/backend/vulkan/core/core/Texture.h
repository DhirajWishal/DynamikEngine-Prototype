#pragma once

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
				VkBuffer* stagingBuffer;
				VkImageLayout imageLayout;
				VkImageView* imageView;
				VkSampler imageSampler;
			};

			struct DMKSkyboxInitInfo {
				std::string path = "";

				VkImage* skyboxImage;
				VkImageLayout imageLayout;
				VkSampler imageSampler;

				VkFormat imageFormat;
				VkDeviceMemory *imageMemory;

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
