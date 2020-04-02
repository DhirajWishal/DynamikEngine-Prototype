#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_TEXTURE_FUNCTIONS_H
#define _DYNAMIK_ADGR_VULKAN_TEXTURE_FUNCTIONS_H

#include "core/vulkan/Renderer Backend Layer/Managers/Objects/VulkanCoreObject.h"
#include "core/vulkan/Renderer Backend Layer/Managers/Objects/VulkanRenderObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanTextureInitInfo {
				std::string path;

				UI32 mipLevels;
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
				UI32 mipLevels;

				UI32 width, height;
			};

			class VulkanTextureFunctions {
				VulkanTextureFunctions() {}
				VulkanTextureFunctions(const VulkanTextureFunctions&) = delete;
				VulkanTextureFunctions operator=(const VulkanTextureFunctions&) = delete;
				VulkanTextureFunctions operator=(VulkanTextureFunctions&&) = delete;

			public:
				static void initializeTexture2D(POINTER<VulkanCoreObject> core, ADGRVulkanTextureInitInfo info, POINTER<VulkanRenderObject> object);
				static void terminateTexture(POINTER<VulkanCoreObject> core, ADGRVulkanTextureContainer container);

				static void generateMipMaps(POINTER<VulkanCoreObject> core, POINTER<ADGRVulkanTextureContainer> container);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_TEXTURE_FUNCTIONS_H
