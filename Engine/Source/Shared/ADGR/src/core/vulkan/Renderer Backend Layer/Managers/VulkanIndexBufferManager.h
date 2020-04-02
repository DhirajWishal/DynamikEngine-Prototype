#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_MANAGER_H

#include "Objects/VulkanCoreObject.h"
#include "Objects/VulkanRenderObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct VulkanCoreObject;
			struct VulkanRenderObject;

			struct ADGRVulkanIndexBufferContainer {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
				UI32 count = 0;
			};

			class VulkanIndexBufferManager {
			public:
				VulkanIndexBufferManager() {}
				~VulkanIndexBufferManager() {}

				void initialize(POINTER<VulkanCoreObject> core, ARRAY<UI32>* indexes, POINTER<VulkanRenderObject> object);
				void terminate(POINTER<VulkanCoreObject> core, ADGRVulkanIndexBufferContainer container);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H
