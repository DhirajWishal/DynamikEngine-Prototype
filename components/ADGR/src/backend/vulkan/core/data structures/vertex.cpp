#include "adgrafx.h"
#include "vertex.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			std::vector<VkVertexInputBindingDescription> Vertex::getBindingDescription(int bindCount) {
				std::vector<VkVertexInputBindingDescription> bindingDescription(bindCount);

				for (int i = 0; i < bindCount; i++) {
					bindingDescription[i].binding = i;
					bindingDescription[i].stride = sizeof(Vertex);
					bindingDescription[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				}

				return bindingDescription;
			}

			//std::vector<std::array<VkVertexInputAttributeDescription, 3>> Vertex::getAttributeDescriptions(int bindCount) {
			//	std::vector<std::array<VkVertexInputAttributeDescription, 3>> attribDesc;
			//
			//	for (int i = 0; i < bindCount; i++) {
			//		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};
			//
			//		attributeDescriptions[0].binding = i;
			//		attributeDescriptions[0].location = (i == 1) ? 0 : 3 ;
			//		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			//		attributeDescriptions[0].offset = offsetof(Vertex, Position);
			//
			//		attributeDescriptions[1].binding = i;
			//		attributeDescriptions[1].location = (i == 1) ? 1 : 4;
			//		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			//		attributeDescriptions[1].offset = offsetof(Vertex, Color);
			//
			//		attributeDescriptions[2].binding = i;
			//		attributeDescriptions[2].location = (i == 1) ? 2 : 5;
			//		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			//		attributeDescriptions[2].offset = offsetof(Vertex, TexCoordinates);
			//
			//		attribDesc.push_back(attributeDescriptions);
			//	}
			//
			//	return attribDesc;
			//}

			std::vector<VkVertexInputAttributeDescription> Vertex::getAttributeDescriptions(int bindCount) {
					std::vector<VkVertexInputAttributeDescription> attributeDescriptions(bindCount * 3);
					int counter = 0;

					for (int i = 0; i < bindCount * 3; i += 3) {
						attributeDescriptions[i].binding = counter;
						attributeDescriptions[i].location = i;
						attributeDescriptions[i].format = VK_FORMAT_R32G32B32_SFLOAT;
						attributeDescriptions[i].offset = offsetof(Vertex, Position);

						attributeDescriptions[i + 1].binding = counter;
						attributeDescriptions[i + 1].location = i + 1;
						attributeDescriptions[i + 1].format = VK_FORMAT_R32G32B32_SFLOAT;
						attributeDescriptions[i + 1].offset = offsetof(Vertex, Color);

						attributeDescriptions[i + 2].binding = counter;
						attributeDescriptions[i + 2].location = i + 2;
						attributeDescriptions[i + 2].format = VK_FORMAT_R32G32_SFLOAT;
						attributeDescriptions[i + 2].offset = offsetof(Vertex, TexCoordinates);

						counter++;
					}
			
				return attributeDescriptions;
			}
		}
	}
}
