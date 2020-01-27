/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#ifdef DMK_USE_VULKAN

#include "queues.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			queueFamilyindices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
				queueFamilyindices indices;

				uint32 queueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

				std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

				int i = 0;
				for (const auto& queueFamily : queueFamilies) {
					if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						indices.graphicsFamily = i;
					}

					VkBool32 presentSupport = false;
					vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

					if (queueFamily.queueCount > 0 && presentSupport)
						indices.presentFamily = i;

					if (indices.isComplete())
						break;

					i++;
				}

				return indices;
			}
		}
	}
}
#endif // DMK_USE_VULKAN
