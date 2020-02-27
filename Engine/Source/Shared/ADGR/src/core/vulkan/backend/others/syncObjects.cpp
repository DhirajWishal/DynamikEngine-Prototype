/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#ifdef DMK_USE_VULKAN

#include "syncObjects.h"
#include "core/vulkan/backend/controlHeader.h"

#include "defines.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			void initSyncObjects(VkDevice device, std::vector<VkSemaphore>* imageAvailableSemaphores,
				std::vector<VkSemaphore>* renderFinishedSemaphores, std::vector<VkFence>* inFlightFences) {
				imageAvailableSemaphores->resize(MAX_FRAMES_IN_FLIGHT);
				renderFinishedSemaphores->resize(MAX_FRAMES_IN_FLIGHT);
				inFlightFences->resize(MAX_FRAMES_IN_FLIGHT);

				VkSemaphoreCreateInfo semaphoreInfo = {};
				semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

				VkFenceCreateInfo fenceInfo = {};
				fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

				for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
					if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores->at(i)) != VK_SUCCESS ||
						vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores->at(i)) != VK_SUCCESS ||
						vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences->at(i)) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create synchronization objects for a frame!");
				}
			}
		}
	}
}

#endif