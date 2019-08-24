#pragma once

#include <glm/glm.hpp>

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct Vertex {
				glm::vec3 Position;
				glm::vec3 Color;
				glm::vec2 TexCoordinates;

				static VkVertexInputBindingDescription getBindingDescription();
				static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();

				bool operator==(const Vertex& other) const {
					return Position == other.Position && Color == other.Color && TexCoordinates == other.TexCoordinates;
				}
			};
		}
	}
}


namespace std {
	using Dynamik::ADGR::core::Vertex;
	using glm::vec2;
	using glm::vec3;

	template<>
	struct hash<Vertex> {
		size_t operator()(Vertex const& vertexes) const {

			return ((hash<vec3>()(vertexes.Position) ^
				(hash<vec3>()(vertexes.Color) << 1)) >> 1) ^
				(hash<vec2>()(vertexes.TexCoordinates) << 1);
		}
	};
}