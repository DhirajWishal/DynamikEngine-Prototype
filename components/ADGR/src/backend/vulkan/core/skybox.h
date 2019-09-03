#pragma once

#include "core/Texture.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKInitCubemapInfo {
				std::string path = "";

				VkFormat textureImageFormat = VK_FORMAT_UNDEFINED;
				VkImage* textureImage = VK_NULL_HANDLE;
				VkDeviceMemory* textureImageMemory = VK_NULL_HANDLE;
				VkImageLayout imageLayout;
				VkSampler imageSampler;

				uint32_t mipLevels = 1;
			};

			struct DMKSkyboxSampelrInitInfo {
				uint32_t mipLevels;

				VkImage skybox;
				VkFormat format;

				VkSampler* sampler;
				VkImageView* imageView;
			};

			class skybox : Texture {
			public:
				skybox() {}
				~skybox() {}

				void initSkybox(DMKSkyboxInitInfo) override;
				void deleteSkybox() override;

				void loadCubemap(DMKInitCubemapInfo initInfo);

				void initSampler(DMKSkyboxSampelrInitInfo info);

			private:
				void generateMipMaps(DMKGenerateMipMapInfo info);

				VkImage cubeMap = VK_NULL_HANDLE;
				VkDeviceMemory cubeMapMemory = VK_NULL_HANDLE;

				VkDescriptorSet skyBox = VK_NULL_HANDLE;

				uint32_t width = NULL;
				uint32_t height = NULL;
			};
		}
	}
}
