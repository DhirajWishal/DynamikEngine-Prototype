#include "adgrafx.h"
#include "VulkanOneTimeCommandBufferManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanOneTimeCommandBufferManager::VulkanOneTimeCommandBufferManager(VulkanCoreObject core, UI32 count)
				: myCount(count), myDevice(core.logicalDevice), myGraphcisQueue(core.graphicsQueue),
				myPresentQueue(core.presentQueue), myCommandPool(core.commandPool)
			{
				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandPool = myCommandPool;
				allocInfo.commandBufferCount = count;

				myCommandBuffers.resize(count);
				vkAllocateCommandBuffers(core.logicalDevice, &allocInfo, myCommandBuffers.data());

				for (I32 i = 0; i < count; i++) {
					VkCommandBufferBeginInfo beginInfo = {};
					beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

					vkBeginCommandBuffer(myCommandBuffers[i], &beginInfo);
				}
			}

			VulkanOneTimeCommandBufferManager::~VulkanOneTimeCommandBufferManager()
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