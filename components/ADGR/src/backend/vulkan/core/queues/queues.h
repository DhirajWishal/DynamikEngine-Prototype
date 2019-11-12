#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

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
