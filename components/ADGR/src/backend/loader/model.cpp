/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		02/08/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "model.h"



#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace std {
	using Dynamik::ADGR::core::vertex;

	template<>
	struct hash<Dynamik::ADGR::core::vertex> {
		size_t operator()(vertex const& vertexs) const {
			using Dynamik::ADGR::core::vertex;
			using glm::vec2;
			using glm::vec3;

			return ((hash<glm::vec3>()(vertexs.pos) ^
				(hash<glm::vec3>()(vertexs.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertexs.texCoord) << 1);
		}
	};
}

namespace Dynamik {
	namespace ADGR {
		namespace core {

			VkVertexInputBindingDescription vertex::getBindingDescription() {
				VkVertexInputBindingDescription bindingDescription = {};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(vertex);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return bindingDescription;
			}

			std::array<VkVertexInputAttributeDescription, 3> vertex::getAttributeDescriptions() {
				std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(vertex, pos);

				attributeDescriptions[1].binding = 0;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(vertex, color);

				attributeDescriptions[2].binding = 0;
				attributeDescriptions[2].location = 2;
				attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[2].offset = offsetof(vertex, texCoord);

				return attributeDescriptions;
			}

			model::model(std::string path) : myPath(path) {

			}

			model::model() {

			}

			model::~model() {

			}

			void model::loadModel() {
				tinyobj::attrib_t attrib;
				std::vector<tinyobj::shape_t> shapes;
				std::vector<tinyobj::material_t> materials;
				std::string warn, err;

				if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
					"E:/Projects/Dynamik Engine/Dynamik/core assets/models/chalet.obj"))
					DMK_CORE_FATAL(warn + err);

				std::unordered_map<vertex, uint32_t> uniqueVertices = {};

				for (const auto& shape : shapes) {
					for (const auto& index : shape.mesh.indices) {
						vertex vertex = {};

						vertex.pos = {
							attrib.vertices[3 * index.vertex_index + 0],
							attrib.vertices[3 * index.vertex_index + 1],
							attrib.vertices[3 * index.vertex_index + 2]
						};

						vertex.texCoord = {
							attrib.texcoords[2 * index.texcoord_index + 0],
							1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
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
}
