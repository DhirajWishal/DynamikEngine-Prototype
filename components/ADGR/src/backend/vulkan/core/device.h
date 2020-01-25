#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_DEVICE_H
#define _DYNAMIK_ADGR_VULKAN_DEVICE_H

#include "core/Device.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class deviceManager : public Device {
			public:
				deviceManager() {}
				~deviceManager() {}

				void init(ADGRVulkanDataContainer* container) override;

			private:
				void initPhysicalDevice(ADGRVulkanDataContainer* container) override;
				void initLogicalDevice(ADGRVulkanDataContainer* container) override;

				bool checkDeviceExtensionSupport(VkPhysicalDevice device);
				bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR* surface);

				const std::vector<const char*> deviceExtensions = {
					VK_KHR_SWAPCHAIN_EXTENSION_NAME
				};
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_DEVICE_H
