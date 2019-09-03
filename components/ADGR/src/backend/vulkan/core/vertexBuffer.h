#pragma once

#include "core/Buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKVertexBufferCreateInfo {
				std::vector<Vertex> vertices = {};

				VkBuffer* buffer = VK_NULL_HANDLE;
				VkDeviceMemory* buffereMemory = VK_NULL_HANDLE;
			};

			struct DMKVertexBufferDeleteInfo {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			class vertexBufferManager : public Buffer {
			public:
				vertexBufferManager() {}
				~vertexBufferManager() {}

				void createVertexBuffer(DMKVertexBufferCreateInfo info);

				void deleteBuffer(DMKVertexBufferDeleteInfo info);
			};
		}
	}
}
