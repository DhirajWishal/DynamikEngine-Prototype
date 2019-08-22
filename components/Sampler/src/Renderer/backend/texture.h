#pragma once

#include "core/Texture.h"

#include "functions/textureFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKInitTextureInfo {
				std::string path;
				
				VkFormat textureImageFormat;
				VkImage* textureImage;
				VkDeviceMemory* textureImageMemory;

				uint32_t mipLevels;
			};

			struct DMKInitTextureImageViewsInfo {
				VkFormat textureImageFormat;
				VkImage textureImage;

				uint32_t mipLevels;
				VkImageAspectFlags flags;

				DMKInitTextureImageViewsInfo(
					VkImageAspectFlags flgs = VK_IMAGE_ASPECT_COLOR_BIT
				) : flags(flgs) {}
			};

			struct DMKInitTextureSamplerInfo {
				VkSampler* textureSampler;

				VkFilter magFilter;
				VkFilter minFilter;

				VkSamplerAddressMode modeU;
				VkSamplerAddressMode modeV;
				VkSamplerAddressMode modeW;

				DMKInitTextureSamplerInfo(
					VkFilter mag = VK_FILTER_LINEAR,
					VkFilter min = VK_FILTER_LINEAR,

					VkSamplerAddressMode mode1 = VK_SAMPLER_ADDRESS_MODE_REPEAT,
					VkSamplerAddressMode mode2 = VK_SAMPLER_ADDRESS_MODE_REPEAT,
					VkSamplerAddressMode mode3 = VK_SAMPLER_ADDRESS_MODE_REPEAT
				) :
					magFilter(mag), minFilter(min),
					modeU(mode1), modeV(mode2), modeW(mode3) {}
			};

			class textureManager : public Texture {
			public:
				textureManager() {}
				~textureManager() {}

				void loadTexture(std::string path) override;
				void initTextureImageView();
				void initTextureSampler();

				void initTexture(DMKInitTextureInfo info);
				void generateMipMaps(DMKGenerateMipMapInfo info) override;
				void initTextureImageViews(DMKInitTextureImageViewsInfo texInfo, DMKCreateImageViewInfo info);
				void initTextureSampler(DMKInitTextureSamplerInfo info);

			private:
				VkDevice* m_device = &device;
				VkImage *m_textureImage = &textureImage;
				VkDeviceMemory *m_textureImageMemory = &textureImageMemory;
				VkImageView *m_textureImageView = &textureImageView;
				VkSampler *m_textureSampler = &textureSampler;

				uint32_t* m_mipLevels = &mipLevels;

				float* minMip = &minMipLevel;
				float* maxMip = &maxMipLevel;
			};
		}
	}
}
