#include "adgrafx.h"
#include "modelManager.h"

#include <tiny_obj_loader.h>

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

			void modelManager::loadModel(DMKModelLoadInfo info) {
				tinyobj::attrib_t attributes;
				std::vector<tinyobj::shape_t> shapes;
				std::vector<tinyobj::material_t> materials;
				std::string warn, err;

				if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &err, info.path.c_str()))
					DMK_CORE_FATAL((warn + err).c_str());

				std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

				for (const auto& shape : shapes) {
					for (const auto& index : shape.mesh.indices) {
						Vertex vertices;

						vertices.Position = {
							attributes.vertices[3 * index.vertex_index + 0] + info.vertexOffset[0],
							attributes.vertices[3 * index.vertex_index + 1] + info.vertexOffset[1],
							attributes.vertices[3 * index.vertex_index + 2] + info.vertexOffset[2]
						};

						vertices.TexCoordinates = {
							attributes.texcoords[2 * index.texcoord_index + 0],
							1.0f - attributes.texcoords[2 * index.texcoord_index + 1]
						};

						vertices.Color = { 1.0f, 1.0f, 1.0f };

						if (uniqueVertices.count(vertices) == 0) {
							uniqueVertices[vertices] = static_cast<uint32_t>(info.vertices->size());
							info.vertices->push_back(vertices);
						}

						info.indices->push_back(uniqueVertices[vertices]);
					}
				}
			}
		}
	}
}

