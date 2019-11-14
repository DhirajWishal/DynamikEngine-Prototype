#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_BUFFER_FUNCTIONS_H
#define _DYNAMIK_ADGR_VULKAN_BUFFER_FUNCTIONS_H

namespace Dynamik {
	namespace ADGR {
		namespace core {
			namespace functions {
				struct DMKCreateBufferInfo {
					VkDevice device;
					VkPhysicalDevice physicalDevice;

					VkBuffer* buffer;
					VkBufferUsageFlagBits usage;
					VkDeviceSize bufferSize;
					VkDeviceMemory* bufferMemory;

					VkMemoryPropertyFlags bufferMemoryPropertyflags;
				};

				void createBuffer(DMKCreateBufferInfo info);

				uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties,
					VkPhysicalDevice physicalDevice);

				void copyBuffer(VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
					VkCommandPool commandPool, VkQueue graphicsQueue);

				VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool);

				void endSingleTimeCommands(VkDevice device, VkCommandPool& commandPool, VkCommandBuffer commandBuffer,
					VkQueue graphicsQueue);

				VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice);

				bool hasStencilComponent(VkFormat format);

				VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
					VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice);

				VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
			}
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_BUFFER_FUNCTIONS_H
