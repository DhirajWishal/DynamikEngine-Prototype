#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_H

#include "CentralDataHub.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"
#include "VulkanQueue.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanVertexBuffer {
			public:
				VulkanVertexBuffer() {}
				~VulkanVertexBuffer() {}

				void initialize(VulkanDevice device, VulkanCommandBuffer commandBuffer, VulkanQueue queue, ARRAY<Vertex>* vertexes);
				void terminate(VulkanDevice device);

				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
				UI32 count = 0;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_H
