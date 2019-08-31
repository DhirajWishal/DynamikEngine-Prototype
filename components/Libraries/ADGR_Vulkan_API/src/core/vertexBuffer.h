#pragma once

#include "core/Buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKVertexBufferCreateInfo {
				std::vector<Vertex> vertices;

				VkBuffer* buffer;
				VkDeviceMemory* buffereMemory;
			};

			struct DMKVertexBufferDeleteInfo {
				VkBuffer buffer;
				VkDeviceMemory bufferMemory;
			};

			class  vertexBufferManager : public Buffer {
			public:
				vertexBufferManager() {}
				~vertexBufferManager() {}

				void createVertexBuffer(DMKVertexBufferCreateInfo info);

				void deleteBuffer(DMKVertexBufferDeleteInfo info);
			};
		}
	}
}
