#include "cdhafx.h"
#include "vertex.h"

namespace Dynamik {
	ARRAY<VkVertexInputBindingDescription> Vertex::getBindingDescription(int bindCount) {
		ARRAY<VkVertexInputBindingDescription> bindingDescription(bindCount);

		for (int i = 0; i < bindCount; i++) {
			bindingDescription[i].binding = i;
			bindingDescription[i].stride = sizeof(Vertex);
			bindingDescription[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}

		return bindingDescription;
	}

	ARRAY<VkVertexInputAttributeDescription> Vertex::getAttributeDescriptions(int bindCount) {
		ARRAY<VkVertexInputAttributeDescription> attributeDescriptions(3);

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, Position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, Color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, TexCoordinates);

		//attributeDescriptions[3].binding = 0;
		//attributeDescriptions[3].location = 3;
		//attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
		//attributeDescriptions[3].offset = offsetof(Vertex, Normals);

		return attributeDescriptions;
	}
}