#pragma once
#ifndef _DYNAMIK_ADGR_UNIFORM_BUFFER_OBJECT_H
#define _DYNAMIK_ADGR_UNIFORM_BUFFER_OBJECT_H

namespace Dynamik {
	namespace ADGR {
		struct UniformBufferObject {
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 proj;
		};

		struct UBO_MPL {
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 proj;
			alignas(16) float lodBias;
		};

		struct UBO_MP {
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 proj;
		};

		struct UBO_MVPL {
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 proj;
			alignas(16) float lodBias;
		};

		struct UBO_MVPC {
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 proj;
			alignas(16) glm::vec3 cameraPos;
		};
	}
}

#endif // !_DYNAMIK_ADGR_UNIFORM_BUFFER_OBJECT_H
