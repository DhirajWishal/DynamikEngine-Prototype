#include "mngafx.h"
#include "objectLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "core/log.h"

namespace Dynamik {
	namespace resources {


		//bool ObjectData::loadObject(std::string path) {
		//	return tinyobj::LoadObj(&attributes, &shapes, &materials, &warnings, &errors, path.c_str());
		//}

		template<typename Alloc>
		void loadObject(std::string& path, std::vector<Alloc>* vertices, std::vector<uint32_t>* indices) {
			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::string warn, err;
			
			if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
				DMK_CORE_FATAL(warn + err);

			std::unordered_map<Alloc, uint32_t> uniqueVertices = {};

			for (const auto& shape : shapes) {
				for (const auto& index : shape.mesh.indices) {
					Alloc vertex = {};

					vertex.pos = {
						myObject.attributes.vertices[3 * index.vertex_index + 0],
						myObject.attributes.vertices[3 * index.vertex_index + 1],
						myObject.attributes.vertices[3 * index.vertex_index + 2]
					};

					vertex.texCoord = {
						myObject.attributes.texcoords[2 * index.texcoord_index + 0],
						1.0f - myObject.attributes.texcoords[2 * index.texcoord_index + 1]
					};

					vertex.color = { 1.0f, 1.0f, 1.0f };

					if (uniqueVertices.count(vertex) == 0) {
						uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
						vertices.push_back(vertex);
					}

					indices.push_back(uniqueVertices[vertex]);
				}
			}
		}
	}
}
