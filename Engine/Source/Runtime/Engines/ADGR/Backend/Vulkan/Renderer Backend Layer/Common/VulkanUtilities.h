#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_UTILITIES_H
#define _DYNAMIK_ADGR_VULKAN_UTILITIES_H

#include <vulkan/vulkan.h>

#include "Public/Array.h"
#include "GameObjectDescriptors.h"
#include "Objects/InternalFormat/InteralFormat.h"
#include "../Graphics/VulkanGraphicsShader.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanUtilitiesTextureInitInfo {
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;

				VkQueue processQueue = VK_NULL_HANDLE;	// graphics/ compute queue
				VkQueue utilityQueue = VK_NULL_HANDLE;	// present/ transfer queue

				UI32 mipLevels = 0;
				UI32 minMipLevels = 0;
				UI32 maxMipLevels = 0;

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

			class VulkanUtilities {
				VulkanUtilities(const VulkanUtilities&) = delete;
				VulkanUtilities(VulkanUtilities&&) = delete;
			public:
				~VulkanUtilities() {}

				static ARRAY<VkVertexInputBindingDescription> getBindingDescription(ARRAY<DMKVertexAttribute> attributes, UI32 bindCount = 1);
				static ARRAY<VkVertexInputAttributeDescription> getAttributeDescriptions(ARRAY<DMKVertexAttribute> attributes, UI32 binding = 1);

				static void updateUniformBuffer(VkDevice device, ARRAY<MAT4> uniformData, VkDeviceMemory uniformBufferMemory);

				static VkFormat vertexAttributeTypeToVkFormat(DMKDataType type);

				static ARRAY<VulkanGraphicsShader> getGraphicsShaders(VkDevice logicalDevice, POINTER<InternalFormat> internalFormat);
				static void terminateGraphicsShaders(VkDevice logicalDevice, ARRAY<VulkanGraphicsShader> shaders);

				static void generateMipMaps(
					VkDevice logicalDevice,
					VkPhysicalDevice physicalDevice,
					VkCommandPool commandPool,
					VkQueue processQueue,
					VkQueue utilityQueue,
					POINTER<ADGRVulkanTextureContainer> container);

				static ADGRVulkanTextureContainer createTexture(ADGRVulkanUtilitiesTextureInitInfo info, Texture textures);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_UTILITIES_H
