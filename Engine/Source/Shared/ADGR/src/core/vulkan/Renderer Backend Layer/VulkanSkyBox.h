#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_SKY_BOX_H
#define _DYNAMIK_ADGR_VULKAN_SKY_BOX_H

#include "Objects/VulkanRenderableObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanSkyBox : public VulkanRenderableObject {
			public:
				VulkanSkyBox(
					VkDevice logicalDevice,
					VkPhysicalDevice physicalDevice,
					VkCommandPool commandPool,
					VkQueue graphicsQueue,
					VkQueue presentQueue)
				: VulkanRenderableObject(
					logicalDevice,
					physicalDevice, commandPool,
					graphicsQueue,
					presentQueue) {}
				~VulkanSkyBox() {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SKY_BOX_H
