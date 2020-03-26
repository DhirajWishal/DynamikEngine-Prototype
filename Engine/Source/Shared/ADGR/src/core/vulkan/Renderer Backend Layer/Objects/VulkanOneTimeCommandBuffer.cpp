#include "adgrafx.h"
#include "VulkanOneTimeCommandBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanOneTimeCommandBuffer::VulkanOneTimeCommandBuffer(VulkanDevice device, VulkanCommandBuffer commandBuffer, VulkanQueue queue, UI32 count)
				: myCount(count), myDevice(device), myQueue(queue), myCommandBuffer(commandBuffer)
			{
				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandPool = commandBuffer.commandPool;
				allocInfo.commandBufferCount = count;

				myCommandBuffers.resize(count);
				vkAllocateCommandBuffers(device.logicalDevice, &allocInfo, myCommandBuffers.data());

				for (I32 i = 0; i < count; i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

					vkBeginCommandBuffer(myCommandBuffers[i], &beginInfo);
				}
			}

			VulkanOneTimeCommandBuffer::~VulkanOneTimeCommandBuffer()
			{
				for (I32 i = 0; i < myCount; i++)
					vkEndCommandBuffer(myCommandBuffers[i]);

				VkSubmitInfo submitInfo = {};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = myCount;
				submitInfo.pCommandBuffers = myCommandBuffers.data();

				vkQueueSubmit(myQueue.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
				vkQueueWaitIdle(myQueue.presentQueue);

				vkFreeCommandBuffers(myDevice.logicalDevice, myCommandBuffer.commandPool, myCount, myCommandBuffers.data());
			}
		}
	}
}