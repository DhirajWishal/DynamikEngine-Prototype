#include "adgrafx.h"
#include "VulkanQueueManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			B1 VulkanQueueManager::isComplete()
			{
				return graphicsFamily.has_value() && presentFamily.has_value();
			}

			VulkanQueueManager VulkanQueueManager::findQueueFamilies(POINTER<VulkanCoreObject> core)
			{
				VulkanQueueManager indices;

				UI32 queueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(core->physicalDevice, &queueFamilyCount, nullptr);

				ARRAY<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(core->physicalDevice, &queueFamilyCount, queueFamilies.data());

				I32 i = 0;
				for (const auto& queueFamily : queueFamilies) {
					if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						indices.graphicsFamily = i;
					}

					VkBool32 presentSupport = false;
					vkGetPhysicalDeviceSurfaceSupportKHR(core->physicalDevice, i, core->surface, &presentSupport);

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