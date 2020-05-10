#include "dmkafx.h"
#include "VulkanOneTimeCommandBuffer.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			VulkanOneTimeCommandBuffer::VulkanOneTimeCommandBuffer(
				VkDevice logicalDevice,
				VkCommandPool commandPool,
				VkQueue processQueue,
				VkQueue utilityQueue) :
				logicalDevice(logicalDevice),
				commandPool(commandPool),
				processQueue(processQueue),
				utilityQueue(utilityQueue)
			{
				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandPool = commandPool;
				allocInfo.commandBufferCount = 1;

				vkAllocateCommandBuffers(logicalDevice, &allocInfo, &buffer);

				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

				vkBeginCommandBuffer(buffer, &beginInfo);
			}

			VulkanOneTimeCommandBuffer::~VulkanOneTimeCommandBuffer()
			{
				vkEndCommandBuffer(buffer);

				VkSubmitInfo submitInfo = {};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = 1;
				submitInfo.pCommandBuffers = &buffer;

				vkQueueSubmit(processQueue, 1, &submitInfo, VK_NULL_HANDLE);
				vkQueueWaitIdle(utilityQueue);

				vkFreeCommandBuffers(logicalDevice, commandPool, 1, &buffer);
			}
		}
	}
}