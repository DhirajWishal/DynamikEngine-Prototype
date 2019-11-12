#include "smpafx.h"
#include "bufferFunctions.h"

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

					if (vkCreateBuffer(info.device, &bufferInfo, nullptr, info.buffer) != VK_SUCCESS) {
						std::runtime_error("failed to create buffer!");
					}

					VkMemoryRequirements memRequirements;
					vkGetBufferMemoryRequirements(info.device, *info.buffer, &memRequirements);

					VkMemoryAllocateInfo allocInfo = {};
					allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
					allocInfo.allocationSize = memRequirements.size;
					allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, info.bufferMemoryPropertyflags,
						info.physicalDevice);

					if (vkAllocateMemory(info.device, &allocInfo, nullptr, info.bufferMemory) != VK_SUCCESS) {
						std::runtime_error("failed to allocate buffer memory!");
					}

					vkBindBufferMemory(info.device, *info.buffer, *info.bufferMemory, 0);
				}

				uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice) {
					VkPhysicalDeviceMemoryProperties memProperties;
					vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

					for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
						if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags
							& properties) == properties)
							return i;

					std::runtime_error("failed to find suitable memory type!");
				}

				void copyBuffer(VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
					VkCommandPool commandPool, VkQueue graphicsQueue) {
					VkCommandBuffer commandBuffer = beginSingleTimeCommands(device, commandPool);

					VkBufferCopy copyRegion = {};
					copyRegion.size = size;
					vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

					endSingleTimeCommands(device, commandPool, commandBuffer, graphicsQueue);
				}

				void copyData() {
				}

				VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool) {
					VkCommandBufferAllocateInfo allocInfo = {};
					allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
					allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
					allocInfo.commandPool = commandPool;
					allocInfo.commandBufferCount = 1;

					VkCommandBuffer commandBuffer;
					vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

					vkBeginCommandBuffer(commandBuffer, &beginInfo);

					return commandBuffer;
				}

				void endSingleTimeCommands(VkDevice device, VkCommandPool& commandPool, VkCommandBuffer commandBuffer,
					VkQueue graphicsQueue) {
					vkEndCommandBuffer(commandBuffer);

					VkSubmitInfo submitInfo = {};
					submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
					submitInfo.commandBufferCount = 1;
					submitInfo.pCommandBuffers = &commandBuffer;

					vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
					vkQueueWaitIdle(graphicsQueue);

					vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
				}

				VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
					VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice) {
					for (VkFormat format : candidates) {
						VkFormatProperties props;

						vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

						if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
							return format;
						}
						else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
							return format;
						}
					}

					std::runtime_error("failed to find supported format!");
				}

				VkFormat findDepthFormat(VkPhysicalDevice physicalDevice) {
					return findSupportedFormat(
						{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
						VK_IMAGE_TILING_OPTIMAL,
						VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
						physicalDevice
					);
				}

				bool hasStencilComponent(VkFormat format) {
					return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
				}

				VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice) {
					VkPhysicalDeviceProperties physicalDeviceProperties;
					vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

					VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts,
						physicalDeviceProperties.limits.framebufferDepthSampleCounts);
					if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
					if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
					if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
					if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
					if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
					if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

					return VK_SAMPLE_COUNT_1_BIT;
				}
			}
		}
	}
}