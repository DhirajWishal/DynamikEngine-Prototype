#pragma once

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
