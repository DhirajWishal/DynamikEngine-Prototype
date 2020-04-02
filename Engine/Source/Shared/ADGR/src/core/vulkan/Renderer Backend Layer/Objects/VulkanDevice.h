#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_DEVICE_H
#define _DYNAMIK_ADGR_VULKAN_DEVICE_H

#include "VulkanInstance.h"
#include "VulkanQueue.h"
#include "VulkanGlobalVariables.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanDevice {
			public:
				VulkanDevice() {}
				~VulkanDevice() {}

				void initialize(VulkanInstance instance, POINTER<VulkanQueue> queue, POINTER<VulkanGlobalVariables> global);
				void terminate();

				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkDevice logicalDevice = VK_NULL_HANDLE;

			private:
				void initPhysicalDevice(VulkanInstance instance, POINTER<VulkanGlobalVariables> global);
				void initLogicalDevice(VulkanInstance instance, POINTER<VulkanQueue> queue);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_DEVICE_H
