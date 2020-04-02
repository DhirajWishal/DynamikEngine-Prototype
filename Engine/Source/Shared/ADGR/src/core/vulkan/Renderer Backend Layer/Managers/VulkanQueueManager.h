#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_QUEUE_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_QUEUE_MANAGER_H

#include <optional>
#include <DataTypesLib/Public/datatypes.h>

#include "Objects/VulkanCoreObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanDeviceManager;
			class VulkanInstanceManager;

			class VulkanQueueManager {
			public:
				VulkanQueueManager() {}
				~VulkanQueueManager() {}

				std::optional<UI32> graphicsFamily;
				std::optional<UI32> presentFamily;

				B1 isComplete();

				static VulkanQueueManager findQueueFamilies(POINTER<VulkanCoreObject> core);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_QUEUE_H
