#include "dmkafx.h"
#include "VulkanComputeOneTimeCommandBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanComputeOneTimeCommandBuffer::VulkanComputeOneTimeCommandBuffer(
				VkDevice device,
				VkCommandPool commandPool,
				VkQueue computeQueue) :
				logicalDevice(device),
				commandPool(commandPool),
				computeQueue(computeQueue)
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

			VulkanComputeOneTimeCommandBuffer::~VulkanComputeOneTimeCommandBuffer()
			{
				vkEndCommandBuffer(buffer);

				VkSubmitInfo submitInfo = {};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = 1;
				submitInfo.pCommandBuffers = &buffer;

				vkQueueSubmit(computeQueue, 1, &submitInfo, VK_NULL_HANDLE);
				vkQueueWaitIdle(computeQueue);

				vkFreeCommandBuffers(logicalDevice, commandPool, 1, &buffer);
			}
		}
	}
}