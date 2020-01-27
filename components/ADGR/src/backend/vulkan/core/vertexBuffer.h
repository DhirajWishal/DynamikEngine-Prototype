#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_H
#ifdef DMK_USE_VULKAN

#include "backend/vulkan/core/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct DMKVertexBufferCreateInfo {
				std::vector<Vertex>* vertices = {};
				//std::vector<PointVertex> pointVertices = {};

				VkBuffer* buffer = VK_NULL_HANDLE;
				VkDeviceMemory* bufferMemory = VK_NULL_HANDLE;
			};

			struct DMKVertexBufferDeleteInfo {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			class vertexBufferManager {
			public:
				vertexBufferManager() {}
				~vertexBufferManager() {}

				void createVertexBuffer(ADGRVulkanDataContainer* container, DMKVertexBufferCreateInfo info);

				void deleteBuffer(ADGRVulkanDataContainer* container, DMKVertexBufferDeleteInfo info);
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif	// !_DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_H
