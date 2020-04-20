#include "dmkafx.h"
#include "modelManager.h"
#include <tiny_obj_loader.h>

#include <cstdlib>

namespace std {
	using Dynamik::Vertex;

	template<>
	struct hash<Dynamik::Vertex> {
		size_t operator()(Vertex const& vertexs) const {
			using Dynamik::Vertex;
			using glm::vec2;
			using glm::vec3;

			return ((hash<glm::vec3>()(vertexs.Position) ^
				(hash<glm::vec3>()(vertexs.Color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertexs.TexCoordinates) << 1);
		}
	};
}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

namespace Dynamik {
	void loadModel(DMKModelLoadInfo info) {
		tinyobj::attrib_t attributes;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &err, (info.path).c_str()))
			//DMK_CORE_FATAL((warn + err).c_str());
			;

		std::unordered_map<Vertex, UI32> uniqueVertices = {};

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertices = {};

				vertices.Position = {
					attributes.vertices[3 * index.vertex_index + 0] + info.vertexOffset[0],
					attributes.vertices[3 * index.vertex_index + 1] + info.vertexOffset[1],
					attributes.vertices[3 * index.vertex_index + 2] + info.vertexOffset[2]
				};

				if (index.texcoord_index != -1)
					vertices.TexCoordinates = {
						attributes.texcoords[2 * index.texcoord_index + 0],
						1.0f - attributes.texcoords[2 * index.texcoord_index + 1]
				};
				else
					vertices.TexCoordinates = {
						0.0f, 0.0f
				};

				vertices.Color = {
					attributes.colors[0],
					attributes.colors[1],
					attributes.colors[2]
				};

				//vertices.Integrity = RandomFloat(0.f, 1.0f);

				if (attributes.normals.size())
				{
					vertices.Normals = {
						attributes.normals[index.normal_index + 0],
						attributes.normals[index.normal_index + 1],
						attributes.normals[index.normal_index + 2]
					};
				}

				if (uniqueVertices.count(vertices) == 0) {
					uniqueVertices[vertices] = static_cast<UI32>(info.vertices->size());
					info.vertices->push_back(vertices);
				}

				info.indices->push_back(uniqueVertices[vertices]);
			}
		}
	}
}