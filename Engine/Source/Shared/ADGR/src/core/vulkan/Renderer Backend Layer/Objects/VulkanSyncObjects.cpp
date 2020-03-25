#include "adgrafx.h"
#include "VulkanSyncObjects.h"
#include "defines.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanSyncObjects::initialize(VulkanDevice device)
			{
				imageAvailables.resize(MAX_FRAMES_IN_FLIGHT);
				renderFinishes.resize(MAX_FRAMES_IN_FLIGHT);
				inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

				VkSemaphoreCreateInfo semaphoreInfo = {};
				semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

				VkFenceCreateInfo fenceInfo = {};
				fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

				for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
					if (vkCreateSemaphore(device.logicalDevice, &semaphoreInfo, nullptr, &imageAvailables.at(i)) != VK_SUCCESS ||
						vkCreateSemaphore(device.logicalDevice, &semaphoreInfo, nullptr, &renderFinishes.at(i)) != VK_SUCCESS ||
						vkCreateFence(device.logicalDevice, &fenceInfo, nullptr, &inFlightFences.at(i)) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create synchronization objects for a frame!");
				}
			}
			
			void VulkanSyncObjects::terminate(VulkanDevice device)
			{
				for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
					vkDestroySemaphore(device.logicalDevice, imageAvailables[i], nullptr);
					vkDestroySemaphore(device.logicalDevice, renderFinishes[i], nullptr);
					vkDestroyFence(device.logicalDevice, inFlightFences[i], nullptr);
				}
			}
		}
	}
}