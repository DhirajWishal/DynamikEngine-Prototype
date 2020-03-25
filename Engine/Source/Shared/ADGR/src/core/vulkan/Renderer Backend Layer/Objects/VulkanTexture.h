#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_TEXTURE_H
#define _DYNAMIK_ADGR_VULKAN_TEXTURE_H

#include <string>
#include "VulkanDevice.h"
#include "VulkanGlobalVariables.h"
#include "VulkanCommandBuffer.h"
#include "VulkanQueue.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanTextureInitInfo {
				VulkanDevice device;
				VulkanGlobalVariables global;
				VulkanCommandBuffer commandBuffer;
				VulkanQueue queue;
				std::string path;

				UI32 mipLevels;
				VkFormat format;
				VkFilter magFilter, minFilter;
				VkSamplerAddressMode modeU, modeV, modeW;
				VkImageAspectFlags aspectFlags;
			};

			class VulkanTexture {
			public:
				VulkanTexture() {}
				~VulkanTexture() {}

				void initialize(ADGRVulkanTextureInitInfo info);
				void terminate(VulkanDevice device);

				VkImage image = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkSampler imageSampler = VK_NULL_HANDLE;
				VkFormat format;
				UI32 mipLevels;

				UI32 width, height;

			private:
				void generateMipMaps(VulkanDevice device, VulkanCommandBuffer commandBuffer, VulkanQueue queue);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_TEXTURE_H
