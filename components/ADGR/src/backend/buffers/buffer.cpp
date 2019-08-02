/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			void createBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
				VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
				VkBufferCreateInfo bufferInfo = {};
				bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferInfo.size = size;
				bufferInfo.usage = usage;
				bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
					throw std::runtime_error("failed to create buffer!");
				}

				VkMemoryRequirements memRequirements;
				vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties,
					physicalDevice);

				if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate buffer memory!");
				}

				vkBindBufferMemory(device, buffer, bufferMemory, 0);
			}

			uint32 findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice) {
				VkPhysicalDeviceMemoryProperties memProperties;
				vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

				for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
					if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags
						& properties) == properties)
						return i;

				throw std::runtime_error("failed to find suitable memory type!");
			}

			void copyBuffer(VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
				VkCommandPool commandPool, VkQueue graphicsQueue) {
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

				VkBufferCopy copyRegion = {};
				copyRegion.size = size;
				vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

				vkEndCommandBuffer(commandBuffer);

				VkSubmitInfo submitInfo = {};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = 1;
				submitInfo.pCommandBuffers = &commandBuffer;

				vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
				vkQueueWaitIdle(graphicsQueue);

				vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
			}

			void copyData() {

			}
		}
	}
}
