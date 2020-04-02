#include "adgrafx.h"
#include "VulkanSyncObjectsManager.h"

#include "defines.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanSyncObjectsManager::initialize(VkDevice device)
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
					if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailables.at(i)) != VK_SUCCESS ||
						vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishes.at(i)) != VK_SUCCESS ||
						vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences.at(i)) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create synchronization objects for a frame!");
				}
			}

			void VulkanSyncObjectsManager::terminate(VkDevice device)
			{
				for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
					vkDestroySemaphore(device, imageAvailables[i], nullptr);
					vkDestroySemaphore(device, renderFinishes[i], nullptr);
					vkDestroyFence(device, inFlightFences[i], nullptr);
				}
			}
		}
	}
}