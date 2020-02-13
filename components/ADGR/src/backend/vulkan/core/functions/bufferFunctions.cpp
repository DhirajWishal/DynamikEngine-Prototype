#include "adgrafx.h"
#if defined(DMK_USE_VULKAN)

#include "bufferFunctions.h"

#include "backend/defines.h"

#include "backend/vulkan/core/commandBufferManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			namespace functions {
				void createBuffer(DMKCreateBufferInfo info) {
					VkBufferCreateInfo bufferInfo = {};
					bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
					bufferInfo.size = info.bufferSize;
					bufferInfo.usage = info.usage;
					bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

					if (vkCreateBuffer(info.device, &bufferInfo, nullptr, info.buffer) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create buffer!");

					VkMemoryRequirements memRequirements;
					vkGetBufferMemoryRequirements(info.device, *info.buffer, &memRequirements);

					VkMemoryAllocateInfo allocInfo = {};
					allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
					allocInfo.allocationSize = memRequirements.size;
					allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, info.bufferMemoryPropertyflags,
						info.physicalDevice);

					if (vkAllocateMemory(info.device, &allocInfo, nullptr, info.bufferMemory) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to allocate buffer memory!");

					vkBindBufferMemory(info.device, *info.buffer, *info.bufferMemory, 0);
				}

				UI32 findMemoryType(UI32 typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice) {
					VkPhysicalDeviceMemoryProperties memProperties;
					vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

					for (UI32 i = 0; i < memProperties.memoryTypeCount; i++)
						if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags
							& properties) == properties)
							return i;

					DMK_CORE_FATAL("failed to find suitable memory type!");
					return 0;
				}

				void copyBuffer(VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
					VkCommandPool commandPool, VkQueue graphicsQueue) {
					oneTimeCommandBufferManager oneTimeCommandBuffer(device, commandPool, graphicsQueue);
					VkCommandBuffer commandBuffer = oneTimeCommandBuffer.getCommandBuffers()[0];

					VkBufferCopy copyRegion = {};
					copyRegion.size = size;
					vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
				}

				void copyData() {
				}

				VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
					VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice) {
					for (VkFormat format : candidates) {
						VkFormatProperties props;

						vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

						if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
							return format;

						else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
							return format;
					}

					DMK_CORE_FATAL("failed to find supported format!");
					return VkFormat();
				}

				VkFormat findDepthFormat(VkPhysicalDevice physicalDevice) {
					return findSupportedFormat(
						{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
						VK_IMAGE_TILING_OPTIMAL,
						VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
						physicalDevice
					);
				}

				B1 hasStencilComponent(VkFormat format) {
					return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
				}

				VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice) {
					VkPhysicalDeviceProperties physicalDeviceProperties;
					vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

					VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts,
						physicalDeviceProperties.limits.framebufferDepthSampleCounts);
					if (counts & VK_SAMPLE_COUNT_64_BIT)  return VK_SAMPLE_COUNT_64_BIT;
					if (counts & VK_SAMPLE_COUNT_32_BIT)  return VK_SAMPLE_COUNT_32_BIT;
					if (counts & VK_SAMPLE_COUNT_16_BIT)  return VK_SAMPLE_COUNT_16_BIT;
					if (counts & VK_SAMPLE_COUNT_8_BIT)  return VK_SAMPLE_COUNT_8_BIT;
					if (counts & VK_SAMPLE_COUNT_4_BIT)  return VK_SAMPLE_COUNT_4_BIT;
					if (counts & VK_SAMPLE_COUNT_2_BIT)  return VK_SAMPLE_COUNT_2_BIT;

					return VK_SAMPLE_COUNT_1_BIT;
				}
			}
		}
	}
}

#endif