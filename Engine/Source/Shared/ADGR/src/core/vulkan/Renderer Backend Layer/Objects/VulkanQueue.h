#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_QUEUE_H
#define _DYNAMIK_ADGR_VULKAN_QUEUE_H

#include <optional>
#include <DataTypesLib/Public/datatypes.h>

#include "VulkanInstance.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanDevice;
			class VulkanInstance;

			class VulkanQueue {
			public:
				VulkanQueue() {}
				~VulkanQueue() {}

				std::optional<UI32> graphicsFamily;
				std::optional<UI32> presentFamily;

				VkQueue graphicsQueue;
				VkQueue presentQueue;

				B1 isComplete();

				static VulkanQueue findQueueFamilies(VulkanDevice device, VulkanInstance instance);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_QUEUE_H
