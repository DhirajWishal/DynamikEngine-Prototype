#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		02/08/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "core/utils/DMK_DataTypes.h"
#include "backend/interface.h"

#include "Data Structures/vertex.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct vertex {
				glm::vec3 pos;
				glm::vec3 color;
				glm::vec2 texCoord;

				static VkVertexInputBindingDescription getBindingDescription();
				static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();

				bool operator==(const vertex& other) const {
					return pos == other.pos && color == other.color && texCoord == other.texCoord;
				}
			};

			class ADGR_API model {
			public:
				model();
				~model();

				void loadModel(std::string& path);

				std::vector<vertex> getVertices() { return vertices; }
				std::vector<uint32> getIndeices() { return indices; }

			private:
				std::string myPath;

				std::vector<vertex> vertices;
				std::vector<uint32> indices;

				float offset = 0.0f;
			};
		}
	}
}
