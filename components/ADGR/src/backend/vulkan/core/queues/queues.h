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

#include "CentralDataHub.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct queueFamilyindices {
				std::optional<uint32> graphicsFamily;
				std::optional<uint32> presentFamily;

				bool isComplete() {
					return graphicsFamily.has_value() && presentFamily.has_value();
				}
			};

			queueFamilyindices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_QUEUES_H
