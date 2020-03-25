#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_SYNC_OBJECTS_H
#define _DYNAMIK_ADGR_VULKAN_SYNC_OBJECTS_H

#include "VulkanDevice.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanSyncObjects {
			public:
				VulkanSyncObjects() {}
				~VulkanSyncObjects() {}

				void initialize(VulkanDevice device);
				void terminate(VulkanDevice device);

				ARRAY<VkSemaphore> imageAvailables;
				ARRAY<VkSemaphore> renderFinishes;
				ARRAY<VkFence> inFlightFences;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SYNC_OBJECTS_H
