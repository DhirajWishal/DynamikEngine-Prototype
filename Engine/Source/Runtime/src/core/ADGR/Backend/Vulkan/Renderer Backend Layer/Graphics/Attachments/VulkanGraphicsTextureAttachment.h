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
				VulkanGraphicsTextureAttachment() { type = ADGRVulkanAttachmentType::ADGR_VULKAN_ATTACHMENT_TYPE_GRAPHICS_TEXTURE_2D; }
				VulkanGraphicsTextureAttachment(ADGRVulkanAttachmentInitInfo info) : VulkanGraphicsAttachment(info) 
				{
					type = ADGRVulkanAttachmentType::ADGR_VULKAN_ATTACHMENT_TYPE_GRAPHICS_TEXTURE_2D;
				}
				~VulkanGraphicsTextureAttachment() {}

				virtual void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos) {}
				virtual void terminateTextures();
				virtual void generateMipMaps(POINTER<ADGRVulkanTextureContainer> container);

				virtual void initializeDescriptor(UI32 destination, VkShaderStageFlags stageFlag) override;

				ARRAY<ADGRVulkanTextureContainer> textureContainers;

			protected:
				virtual void _initializeDescriptorPool() override;
				virtual void _initializeDescriptorSets(UI32 destination = 0) override;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_GRAPHICS_TEXTURE_ATTACHMENT_H
