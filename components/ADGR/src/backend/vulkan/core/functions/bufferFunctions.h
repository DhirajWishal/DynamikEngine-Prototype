#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_BUFFER_FUNCTIONS_H
#define _DYNAMIK_ADGR_VULKAN_BUFFER_FUNCTIONS_H

namespace Dynamik {
	namespace ADGR {
		namespace core {
			namespace functions {
				struct DMKCreateBufferInfo {
					VkDevice device = VK_NULL_HANDLE;
					VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

					VkBuffer* buffer = nullptr;
					VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
					VkDeviceSize bufferSize = 0;
					VkDeviceMemory* bufferMemory = nullptr;

					VkMemoryPropertyFlags bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				};

				void createBuffer(DMKCreateBufferInfo info);

				uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties,
					VkPhysicalDevice physicalDevice);

				void copyBuffer(VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
					VkCommandPool commandPool, VkQueue graphicsQueue);

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
