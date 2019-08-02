#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "src/backend/interface.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

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

			class ADGR_API vertexBuffer {
			public:
				vertexBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
					VkBuffer* vertexBuffer, VkDeviceMemory* vertexBufferMemory);
				~vertexBuffer() {}

				void initBuffer(VkCommandPool commandPool, VkQueue graphicsQueue);
				void deleteVertexBuffer();

				VkBuffer getVertexBuffer() const { return *myVertexBuffer; }

			private:
				VkDevice* myDevice;
				VkPhysicalDevice* myPhysicalDevice;
				VkBuffer* myVertexBuffer;
				VkDeviceMemory* myVertexBufferMemory;
			};
		}
	}
}


