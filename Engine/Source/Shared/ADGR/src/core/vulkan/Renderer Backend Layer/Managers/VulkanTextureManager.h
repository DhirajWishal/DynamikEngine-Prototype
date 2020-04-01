#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_TEXTURE_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_TEXTURE_MANAGER_H

#include <string>
#include "Objects/VulkanCoreObject.h"
#include "Objects/VulkanRenderObject.h"

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

			class VulkanTextureManager {
			public:
				VulkanTextureManager() {}
				~VulkanTextureManager() {}

				void initialize(POINTER<VulkanCoreObject> core, ADGRVulkanTextureInitInfo info, POINTER<VulkanRenderObject> object);
				void terminate(POINTER<VulkanCoreObject> core, ADGRVulkanTextureContainer container);

			private:
				void generateMipMaps(POINTER<VulkanCoreObject> core, POINTER<ADGRVulkanTextureContainer> container);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_TEXTURE_H
