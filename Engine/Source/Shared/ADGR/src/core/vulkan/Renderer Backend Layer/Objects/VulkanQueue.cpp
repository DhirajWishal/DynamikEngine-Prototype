#include "adgrafx.h"
#include "VulkanQueue.h"

#include "VulkanDevice.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			B1 VulkanQueue::isComplete()
			{
				return graphicsFamily.has_value() && presentFamily.has_value();
			}

			VulkanQueue VulkanQueue::findQueueFamilies(VulkanDevice device, VulkanInstance instance)
			{
				VulkanQueue indices;

				UI32 queueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(device.physicalDevice, &queueFamilyCount, nullptr);

				ARRAY<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(device.physicalDevice, &queueFamilyCount, queueFamilies.data());

				I32 i = 0;
				for (const auto& queueFamily : queueFamilies) {
					if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						indices.graphicsFamily = i;
					}

					VkBool32 presentSupport = false;
					vkGetPhysicalDeviceSurfaceSupportKHR(device.physicalDevice, i, instance.surface, &presentSupport);

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