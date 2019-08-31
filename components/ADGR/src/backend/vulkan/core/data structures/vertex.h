#pragma once

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct Vertex {
				glm::vec3 Position;
				glm::vec3 Color;
				glm::vec2 TexCoordinates;

				static std::vector<VkVertexInputBindingDescription> getBindingDescription(int bindCount);
				//static std::vector<std::array<VkVertexInputAttributeDescription, 3>> getAttributeDescriptions(int bindCount);
				//static std::array<VkVertexInputAttributeDescription, 6> getAttributeDescriptions(int bindCount);
				static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(int bindCount);

				bool operator==(const Vertex& other) const {
					return Position == other.Position && Color == other.Color && TexCoordinates == other.TexCoordinates;
				}
			};
		}
	}
}

//namespace std {
//
//	template<>
//	struct hash<Dynamik::ADGR::core::Vertex> {
//		size_t operator()(Dynamik::ADGR::core::Vertex const& vertexes) const {
//
//			return ((hash<glm::vec3>()(vertexes.Position) ^
//				(hash<glm::vec3>()(vertexes.Color) << 1)) >> 1) ^
//				(hash<glm::vec2>()(vertexes.TexCoordinates) << 1);
//		}
//	};
//}
