#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_DEVICE_H
#define _DYNAMIK_ADGR_VULKAN_DEVICE_H

#include "VulkanInstance.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanDevice {
			public:
				VulkanDevice() {}
				~VulkanDevice() {}

				void initialize(VulkanInstance instance);
				void terminate();

				VkPhysicalDevice getPhysicalDevice() { return myPhysicalDevice; }
				POINTER<VkPhysicalDevice> getPhysicalDeviceAddress() { return &myPhysicalDevice; }

				VkDevice getLogicalDevice() { return myLogicalDevice; }
				POINTER<VkDevice> getLogicalDeviceAddress() { return &myLogicalDevice; }

			private:
				void initPhysicalDevice(VulkanInstance instance);
				void initLogicalDevice(VulkanInstance instance);

				VkPhysicalDevice myPhysicalDevice = VK_NULL_HANDLE;
				VkDevice myLogicalDevice = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_DEVICE_H
