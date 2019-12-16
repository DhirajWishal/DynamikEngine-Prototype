#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_H

#include "core/Buffer.h"

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

			class vertexBufferManager : public Buffer {
			public:
				vertexBufferManager() {}
				~vertexBufferManager() {}

				void createVertexBuffer(ADGRVulkanDataContainer* container, DMKVertexBufferCreateInfo info);

				void deleteBuffer(ADGRVulkanDataContainer* container, DMKVertexBufferDeleteInfo info);
			};
		}
	}
}

#endif	// !_DYNAMIK_ADGR_VULKAN_VERTEX_BUFFER_H
