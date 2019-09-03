#pragma once

#include "Core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKModelLoadInfo {
				std::string path = "";

				std::vector<Vertex>* vertices = {};
				std::vector<uint32_t>* indices = {};

				std::vector<float> vertexOffset = { 0.0f, 0.0f, 0.0f };	// x, y, z
			};

			class Model : public Core {
			public:
				Model() {}
				virtual ~Model() {}

				virtual void loadModel(DMKModelLoadInfo info) {}
			};
		}
	}
}
