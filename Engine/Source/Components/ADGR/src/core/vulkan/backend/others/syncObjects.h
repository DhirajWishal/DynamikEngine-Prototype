#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_ADGR_VULKAN_SYNC_OBJECTS_H
#define _DYNAMIK_ADGR_VULKAN_SYNC_OBJECTS_H
#ifdef DMK_USE_VULKAN

namespace Dynamik {
	namespace ADGR {
		namespace core {
			void initSyncObjects(VkDevice device, std::vector<VkSemaphore>* imageAvailableSemaphores,
				std::vector<VkSemaphore>* renderFinishedSemaphores, std::vector<VkFence>* inFlightFences);
		}
	}
}

#endif 
#endif // !_DYNAMIK_ADGR_VULKAN_SYNC_OBJECTS_H
