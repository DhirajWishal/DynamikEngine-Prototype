#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class DMK_API vertexBuffer : public Buffer {
			public:
				vertexBuffer(VkBuffer* vertexBuffer, VkDeviceMemory* vertexBufferMemory, uint32 type);
				~vertexBuffer() {}

				void initBuffer(VkQueue graphicsQueue) override;
				void deleteBuffer();

				void copyData();
				void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
					VkMemoryPropertyFlags properties, VkBuffer& buffer,
					VkDeviceMemory& bufferMemory);
				void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
					VkQueue graphicsQueue);

			private:
				uint32 findMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties);

			};

			struct vertex {
				glm::vec2 position;
				glm::vec3 color;

				static VkVertexInputBindingDescription getBindingDescription();
				static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
			};

			const std::vector<vertex> vertices = {
				{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
				{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
				{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
				{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
			};
		}
	}
}


