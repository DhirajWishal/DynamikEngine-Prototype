#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_TEXTURE_ATTACHMENT_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_TEXTURE_ATTACHMENT_H

#include "VulkanGraphicsAttachment.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanTextureInitInfo {
				std::string path = "";

				UI32 mipLevels = 0;
				UI32 minMipLevels = 0;
				UI32 maxMipLevels = 0;

				VkFormat format;
				VkFilter magFilter, minFilter;
				VkSamplerAddressMode modeU, modeV, modeW;
				VkImageAspectFlags aspectFlags;
			};

			struct ADGRVulkanTextureContainer {
				VkImage image = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkSampler imageSampler = VK_NULL_HANDLE;
				VkFormat format;
				UI32 mipLevels = 0;

				UI32 width = 0, height = 0;
			};

			class VulkanGraphicsTextureAttachment : public VulkanGraphicsAttachment {
			public:
				VulkanGraphicsTextureAttachment() {}
				VulkanGraphicsTextureAttachment(ADGRVulkanAttachmentInitInfo info) : VulkanGraphicsAttachment(info) {}
				~VulkanGraphicsTextureAttachment() {}

				virtual void initializeTexture(ADGRVulkanTextureInitInfo info) {}
				virtual void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos) {}
				virtual void terminateTexture() {}
				virtual void terminateTextures() {}

				ARRAY<ADGRVulkanTextureContainer> textureContainers;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_GRAPHICS_TEXTURE_ATTACHMENT_H
