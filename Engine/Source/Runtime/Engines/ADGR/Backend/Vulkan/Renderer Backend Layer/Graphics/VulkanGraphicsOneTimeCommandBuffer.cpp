#include "dmkafx.h"
#include "VulkanGraphicsOneTimeCommandBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanGraphicsOneTimeCommandBuffer::VulkanGraphicsOneTimeCommandBuffer(
				VkDevice logicalDevice,
				VkCommandPool commandPool,
				VkQueue graphicsQueue,
				VkQueue presentQueue,
				UI32 count)
				: myCount(count), myDevice(logicalDevice), myGraphcisQueue(graphicsQueue),
				myPresentQueue(presentQueue), myCommandPool(commandPool)
			{
				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandPool = myCommandPool;
				allocInfo.commandBufferCount = count;

				myCommandBuffers.resize(count);
				vkAllocateCommandBuffers(logicalDevice, &allocInfo, myCommandBuffers.data());

				for (I32 i = 0; i < count; i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

					vkBeginCommandBuffer(myCommandBuffers[i], &beginInfo);
				}
			}

			VulkanGraphicsOneTimeCommandBuffer::~VulkanGraphicsOneTimeCommandBuffer()
			{
				for (I32 i = 0; i < myCount; i++)
					vkEndCommandBuffer(myCommandBuffers[i]);

				VkSubmitInfo submitInfo = {};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = myCount;
				submitInfo.pCommandBuffers = myCommandBuffers.data();

				vkQueueSubmit(myGraphcisQueue, 1, &submitInfo, VK_NULL_HANDLE);
				vkQueueWaitIdle(myPresentQueue);

				vkFreeCommandBuffers(myDevice, myCommandPool, myCount, myCommandBuffers.data());
			}
		}
	}
}