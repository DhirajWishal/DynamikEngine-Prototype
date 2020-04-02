#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_DEVICE_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_DEVICE_MANAGER_H

#include "Objects/VulkanCoreObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanDeviceManager {
			public:
				VulkanDeviceManager() {}
				~VulkanDeviceManager() {}

				void initialize(POINTER<VulkanCoreObject> core);
				void terminate(POINTER<VulkanCoreObject> core);

			private:
				void initPhysicalDevice(POINTER<VulkanCoreObject> core);
				void initLogicalDevice(POINTER<VulkanCoreObject> core);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_DEVICE_H
