#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_DEVICE_H
#define _DYNAMIK_ADGR_VULKAN_DEVICE_H
#ifdef DMK_USE_VULKAN

#include "backend/vulkan/core/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class deviceManager {
			public:
				deviceManager() {}
				~deviceManager() {}

				void init(ADGRVulkanDataContainer* container);

			private:
				void initPhysicalDevice(ADGRVulkanDataContainer* container);
				void initLogicalDevice(ADGRVulkanDataContainer* container);

				bool checkDeviceExtensionSupport(VkPhysicalDevice device);
				bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR* surface);

				const std::vector<const char*> deviceExtensions = {
					VK_KHR_SWAPCHAIN_EXTENSION_NAME
				};
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_DEVICE_H
