#pragma once

#include "core/Buffer.h"

#include "data structures/vertex.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKVertexBufferCreateInfo {
				std::vector<Vertex> vertices;

				VkBuffer* buffer;
				VkDeviceMemory* buffereMemory;
			};

			class vertexBufferManager : public Buffer {
			public:
				vertexBufferManager() {}
				~vertexBufferManager() {}

				void createVertexBuffer(DMKVertexBufferCreateInfo info);
			};
		}
	}
}
