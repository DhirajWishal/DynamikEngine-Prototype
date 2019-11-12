#pragma once

#include <glm/glm.hpp>

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct UniformBufferObject {
				alignas(16) glm::mat4 model;
				glm::mat4 view;
				glm::mat4 proj;
			};
		}
	}
}
