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

				void initTexture(ADGRVulkanDataContainer* container, DMKInitTextureInfo info);
				void generateMipMaps(ADGRVulkanDataContainer* container, DMKGenerateMipMapInfo info) override;
				void initTextureImageViews(ADGRVulkanDataContainer* container, DMKInitTextureImageViewsInfo texInfo, DMKCreateImageViewInfo info);
				void initTextureSampler(ADGRVulkanDataContainer* container, DMKInitTextureSamplerInfo info);

				void deleteTexture(ADGRVulkanDataContainer* container, DMKTextureDeleteInfo info);
			};
		}
	}
}
