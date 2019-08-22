#include "smpafx.h"
#include "vertex.h"

namespace std {
	using Dynamik::ADGR::core::Vertex;

	template<>
	struct hash<Dynamik::ADGR::core::Vertex> {
		size_t operator()(Vertex const& vertexs) const {
			using Dynamik::ADGR::core::Vertex;
			using glm::vec2;
			using glm::vec3;

			return ((hash<glm::vec3>()(vertexs.Position) ^
				(hash<glm::vec3>()(vertexs.Color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertexs.TexCoordinates) << 1);
		}
	};
}

namespace Dynamik {
	namespace ADGR {
		namespace core {

			VkVertexInputBindingDescription Vertex::getBindingDescription() {
				VkVertexInputBindingDescription bindingDescription = {};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(Vertex);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return bindingDescription;
			}

			std::array<VkVertexInputAttributeDescription, 3> Vertex::getAttributeDescriptions() {
				std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

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

				return attributeDescriptions;
			}
		}
	}
}