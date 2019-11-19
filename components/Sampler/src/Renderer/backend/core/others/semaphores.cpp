/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "semaphores.h"
#include "backend/vulkan/core/controlHeader.h"

#include "backend/defines.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			void createSemaphores(VkDevice device, std::vector<VkSemaphore>* imageAvailableSemaphores,
				std::vector<VkSemaphore>* renderFinishedSemaphores) {
				imageAvailableSemaphores->resize(MAX_FRAMES_IN_FLIGHT);
				renderFinishedSemaphores->resize(MAX_FRAMES_IN_FLIGHT);

				VkSemaphoreCreateInfo semaphoreInfo = {};
				semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

				for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
					if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores->at(i))
						!= VK_SUCCESS ||
						vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores->at(i))
						!= VK_SUCCESS)
						DMK_CORE_FATAL("failed to create semaphores for a frame!");
				}
			}
		}
	}
}