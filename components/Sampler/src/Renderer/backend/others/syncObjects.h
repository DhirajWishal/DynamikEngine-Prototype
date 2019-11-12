#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

namespace Dynamik {
	namespace ADGR {
		namespace core {

			void initSyncObjects(VkDevice device, std::vector<VkSemaphore>* imageAvailableSemaphores,
				std::vector<VkSemaphore>* renderFinishedSemaphores, std::vector<VkFence>* inFlightFences);
		}
	}
}
