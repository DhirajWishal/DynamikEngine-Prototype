#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_UTILITIES_H
#define _DYNAMIK_ADGR_VULKAN_UTILITIES_H

#include <vulkan/vulkan.h>

#include "Public/Array.h"
#include "GameObjectDescriptors.h"
#include "Objects/InternalFormat/InternalFormat.h"
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

			struct ADGRVulkanCreateBufferInfo {
				VkBuffer* buffer = nullptr;
				VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				VkDeviceSize bufferSize = 0;
				VkDeviceMemory* bufferMemory = nullptr;

				VkMemoryPropertyFlags bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			};

			struct ADGRVulkanCreateImageInfo {
				UI32 width = 0;
				UI32 height = 0;

				VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

				VkFormat format = VK_FORMAT_UNDEFINED;
				VkImage* image = nullptr;
				VkDeviceMemory* imageMemory = nullptr;

				VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
				VkImageUsageFlags usage = VK_IMAGE_CREATE_SPARSE_BINDING_BIT;
				VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				VkSampleCountFlagBits numSamples = VK_SAMPLE_COUNT_1_BIT;

				UI32 mipLevels = 0;

				UI32 arrayLayers = 2;

				VkImageCreateFlags flags = VK_IMAGE_CREATE_SPARSE_BINDING_BIT;
			};

			struct ADGRVulkanTransitionImageLayoutInfo {
				VkImage image;
				VkFormat format;
				VkImageLayout oldLayout;
				VkImageLayout newLayout;

				UI32 mipLevels;
				UI32 layerCount;

				UI32 arrayLayers = 0;
			};

			struct ADGRVulkanCreateImageViewInfo {
				VkImage image;
				VkFormat format;
				VkImageAspectFlags aspectFlags;
				VkImageView* textureImageView;

				VkComponentMapping component = {};

				VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D;

				UI32 mipLevels;
				UI32 layerCount = 1;
			};

			struct ADGRVulkanCopyBufferToImageInfo {
				VkBuffer buffer;
				VkImage image;
				VkImageLayout destinationImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

				UI32 width;
				UI32 height;

				UI32 baseArrayCount = 0;
				UI32 layerCount = 1;
			};

			struct ADGRVulkanTextureSamplerInitInfo {
				VkFilter magFilter;
				VkFilter minFilter;
				VkSamplerAddressMode modeU;
				VkSamplerAddressMode modeV;
				VkSamplerAddressMode modeW;

				F32 minMipLevels;
				F32 maxMipLevels;

				VkBool32 anisotrophyEnable = VK_FALSE;
				F32 maxAnisotrophy = 16;
				VkBorderColor borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
				VkBool32 unnormalizedCoordinates = VK_FALSE;
				VkBool32 compareEnable = VK_FALSE;
				VkCompareOp compareOp = VK_COMPARE_OP_ALWAYS;
				VkSamplerMipmapMode mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				F32 mipLoadBias = 0;
			};

			struct ADGRVulkanUnformBufferContainer {
				ARRAY<VkBuffer> buffers;
				ARRAY<VkDeviceMemory> bufferMemories;
			};

			class VulkanUtilities {
				VulkanUtilities(const VulkanUtilities&) = delete;
				VulkanUtilities(VulkanUtilities&&) = delete;
			public:
				~VulkanUtilities() {}

				/* GLOBAL */
				static VkFormat getFormat(DMKFormat format);

				/* VERTEX FUNCTIONS */
				static ARRAY<VkVertexInputBindingDescription> getBindingDescription(ARRAY<DMKVertexAttribute> attributes, UI32 bindCount = 1);
				static ARRAY<VkVertexInputAttributeDescription> getAttributeDescriptions(ARRAY<DMKVertexAttribute> attributes, UI32 binding = 0);
				static VkFormat vertexAttributeTypeToVkFormat(DMKDataType type);

				/* SHADER FUNCTIONS */
				static ARRAY<VulkanGraphicsShader> getGraphicsShaders(VkDevice logicalDevice, POINTER<InternalFormat> internalFormat);
				static void terminateGraphicsShaders(VkDevice logicalDevice, ARRAY<VulkanGraphicsShader> shaders);

				/* BUFFER FUNCTIONS */
				static void generateMipMaps(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue processQueue, VkQueue utilityQueue, POINTER<ADGRVulkanTextureContainer> container);
				static ADGRVulkanTextureContainer createTexture(ADGRVulkanUtilitiesTextureInitInfo info, Texture textures);

				static void createBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, ADGRVulkanCreateBufferInfo info);
				static UI32 findMemoryType(UI32 typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);
				static VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice);
				static B1 hasStencilComponent(VkFormat format);
				static VkFormat findSupportedFormat(const ARRAY<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice);
				static VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
				static void copyBuffer(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkQueue presentQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

				/* TEXTURE FUNCTIONS */
				static void createImage(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, ADGRVulkanCreateImageInfo info);
				static void transitionImageLayout(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkQueue presentQueue, ADGRVulkanTransitionImageLayoutInfo info);
				static void transitionImageLayout(VkDevice logicalDevice, VkCommandBuffer commandBuffer, ADGRVulkanTransitionImageLayoutInfo info);
				static VkImageView createImageView(VkDevice device, ADGRVulkanCreateImageViewInfo info);
				static void copyBufferToImage(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkQueue presentQueue, ADGRVulkanCopyBufferToImageInfo info);
				static void copyBufferToImage(VkDevice logicalDevice, VkCommandBuffer commandBuffer, ADGRVulkanCopyBufferToImageInfo info);
				static void copyBufferToImageOverride(VkDevice logicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkQueue presentQueue, ADGRVulkanCopyBufferToImageInfo info, ARRAY<VkBufferImageCopy> copyRegions);
				static void copyBufferToImageOverride(VkDevice logicalDevice, VkCommandBuffer commandBuffer, ADGRVulkanCopyBufferToImageInfo info, ARRAY<VkBufferImageCopy> copyRegions);
				static VkSampler createImageSampler(VkDevice logicalDevice, ADGRVulkanTextureSamplerInitInfo info);

				/* UNIFORM BUFFER FUNCTIONS */
				static ADGRVulkanUnformBufferContainer createUniformBuffers(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize, UI32 count);
				static void updateUniformBuffer(VkDevice device, ARRAY<MAT4> uniformData, VkDeviceMemory uniformBufferMemory, DMKUniformBufferObjectDescriptor descriptor);
				static VkDescriptorType getDescriptorType(DMKUniformType type);
				static VkShaderStageFlagBits getDescriptorFlag(DMKAttributeLocation location);
				static ARRAY<VkDescriptorSetLayoutBinding> getDescriptorSetBindings(ARRAY<DMKUniformBufferObjectDescriptor> descriptors);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_UTILITIES_H
