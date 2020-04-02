#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COLOR_BUFFER_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_COLOR_BUFFER_MANAGER_H

#include "Objects/VulkanCoreObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanColorBufferContainer {
				VkImage image = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkImageView imageView;
			};

			class VulkanColorBufferManager {
			public:
				VulkanColorBufferManager() {}
				~VulkanColorBufferManager() {}

				ADGRVulkanColorBufferContainer initialize(POINTER<VulkanCoreObject> core);
				void terminate(POINTER<VulkanCoreObject> core, ADGRVulkanColorBufferContainer container);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COLOR_BUFFER_H
