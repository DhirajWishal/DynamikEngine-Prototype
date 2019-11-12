#pragma once

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct Vertex {
				glm::vec3 Position;
				glm::vec3 Color;
				glm::vec2 TexCoordinates;

				static std::vector<VkVertexInputBindingDescription> getBindingDescription(int bindCount);
				static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(int bindCount);

				bool operator==(const Vertex& other) const {
					return Position == other.Position && Color == other.Color && TexCoordinates == other.TexCoordinates;
				}
			};

			struct PointVertex {
				glm::vec3 Position;
				glm::vec3 Color;

				static std::vector<VkVertexInputBindingDescription> getBindingDescription(int bindCount);
				static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(int bindCount);

				bool operator==(const Vertex& other) const {
					return Position == other.Position && Color == other.Color;
				}
			};
		}
	}
}
