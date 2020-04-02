#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_SYNC_OBJECTS_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_SYNC_OBJECTS_MANAGER_H

#include "VulkanDeviceManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanSyncObjectsManager {
			public:
				VulkanSyncObjectsManager() {}
				~VulkanSyncObjectsManager() {}

				void initialize(VkDevice device);
				void terminate(VkDevice device);

				ARRAY<VkSemaphore> imageAvailables;
				ARRAY<VkSemaphore> renderFinishes;
				ARRAY<VkFence> inFlightFences;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SYNC_OBJECTS_H
