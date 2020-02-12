#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_ADGR_VULKAN_QUEUES_H
#define _DYNAMIK_ADGR_VULKAN_QUEUES_H
#ifdef DMK_USE_VULKAN

#include "CentralDataHub.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct queueFamilyindices {
				std::optional<UI32> graphicsFamily;
				std::optional<UI32> presentFamily;

				B1 isComplete() {
					return graphicsFamily.has_value() && presentFamily.has_value();
				}
			};

			queueFamilyindices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
		}
	}
}

#endif // DMK_USE_VULKAN
#endif // !_DYNAMIK_ADGR_VULKAN_QUEUES_H
