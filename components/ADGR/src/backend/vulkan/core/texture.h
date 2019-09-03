#pragma once

#include "core/Texture.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

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

				DMKInitTextureImageViewsInfo(
					VkImageAspectFlags flgs = VK_IMAGE_ASPECT_COLOR_BIT
				) : flags(flgs) {}
			};

			struct DMKInitTextureSamplerInfo {
				VkSampler* textureSampler = VK_NULL_HANDLE;
				uint32_t mipLevel = 1;

				VkFilter magFilter = VK_FILTER_NEAREST;
				VkFilter minFilter = VK_FILTER_NEAREST;

				VkSamplerAddressMode modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				VkSamplerAddressMode modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				VkSamplerAddressMode modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

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

			struct DMKTextureDeleteInfo {
				VkSampler sampler = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
				VkImage texture = VK_NULL_HANDLE;
				VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
			};

			class textureManager : public Texture {
			public:
				textureManager() {}
				~textureManager() {}

				void loadTexture(std::string path, resource::TextureType type) override;
				void initTextureImageView();
				void initTextureSampler();

				void initTexture(DMKInitTextureInfo info);
				void generateMipMaps(DMKGenerateMipMapInfo info) override;
				void initTextureImageViews(DMKInitTextureImageViewsInfo texInfo, DMKCreateImageViewInfo info);
				void initTextureSampler(DMKInitTextureSamplerInfo info);

				void deleteTexture(DMKTextureDeleteInfo info);

			private:
				VkDevice* m_device = &device;
				VkImage* m_textureImage = &textureImage;
				VkDeviceMemory* m_textureImageMemory = &textureImageMemory;
				VkImageView* m_textureImageView = &textureImageView;
				VkSampler* m_textureSampler = &textureSampler;

				uint32_t* m_mipLevels = &mipLevels;

				float* minMip = &minMipLevel;
				float* maxMip = &maxMipLevel;
			};
		}
	}
}
