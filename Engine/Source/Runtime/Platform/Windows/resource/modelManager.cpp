#include "dmkafx.h"
#include "modelManager.h"
#include <tiny_obj_loader.h>

namespace std {
	template<>
	struct hash<Dynamik::MeshPointStore> {
		size_t operator() (const Dynamik::MeshPointStore& other)
		{
			return (((hash<Dynamik::F32>()(other.position[0] * other.position[1] * other.position[2])
				^ (hash<Dynamik::F32>()(other.color[0] * other.color[1] * other.color[2]) << 1)) >> 1 ^
				(hash<Dynamik::F32>()(other.textureCoordinate[0] * other.textureCoordinate[1]) >> 1)) * other.integrity);
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
			DMK_CORE_FATAL((warn + err).c_str());

		MeshPointStore _store;
		std::unordered_map<MeshPointStore, UI32> uniqueVertices = {};

		for (const auto& shape : shapes) {
			Mesh _mesh;
			for (const auto& index : shape.mesh.indices) {
				_store.position = {
					attributes.vertices[3 * index.vertex_index + 0] + info.vertexOffset[0],
					attributes.vertices[3 * index.vertex_index + 1] + info.vertexOffset[1],
					attributes.vertices[3 * index.vertex_index + 2] + info.vertexOffset[2]
				};

				_store.textureCoordinate = {
						attributes.texcoords[2 * index.texcoord_index + 0],
						1.0f - attributes.texcoords[2 * index.texcoord_index + 1]
				};

				_store.color = {
					attributes.colors[0],
					attributes.colors[1],
					attributes.colors[2]
				};

				_store.integrity = 1.0f;

				_store.normal = {
					attributes.normals[index.normal_index + 0],
					attributes.normals[index.normal_index + 1],
					attributes.normals[index.normal_index + 2]
				};

				if (uniqueVertices.count(_store) == 0) {
					uniqueVertices[_store] = static_cast<UI32>(_mesh.vertexDataStore.size());
					_mesh.vertexDataStore.push_back(_store);
				}

				_mesh.indexes.push_back(uniqueVertices[_store]);
			}

			info.meshes->pushBack(_mesh);
		}
	}
}