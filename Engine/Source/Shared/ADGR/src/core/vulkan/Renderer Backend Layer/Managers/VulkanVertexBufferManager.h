#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_MANAGER_H

#include "Objects/VulkanCoreObject.h"
#include "Objects/VulkanRenderObject.h"

#include "CentralDataHub.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct VulkanCoreObject;
			struct VulkanRenderObject;

			struct ADGRVulkanVertexBufferContainer {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
				UI32 count = 0;
			};

			class VulkanVertexBufferManager {
			public:
				VulkanVertexBufferManager() {}
				~VulkanVertexBufferManager() {}

				void initialize(POINTER<VulkanCoreObject> core, ARRAY<Vertex>* vertexes, POINTER<VulkanRenderObject> object);
				void terminate(POINTER<VulkanCoreObject> core, ADGRVulkanVertexBufferContainer container);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_H
