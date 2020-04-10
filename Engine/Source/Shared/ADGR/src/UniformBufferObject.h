#pragma once
#ifndef _DYNAMIK_ADGR_UNIFORM_BUFFER_OBJECT_H
#define _DYNAMIK_ADGR_UNIFORM_BUFFER_OBJECT_H

// Maximum number of bones per mesh
// Must not be higher than same const in skinning shader
#define MAX_BONES 64

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

		struct UBO_SKELETAL {
			glm::mat4 projection;
			glm::mat4 view;
			glm::mat4 model;
			glm::mat4 bones[MAX_BONES];
			glm::vec4 lightPos = glm::vec4(0.0f, -250.0f, 250.0f, 1.0);
			glm::vec4 viewPos;
		};

		enum class DMKUniformBufferType {
			DMK_UNIFORM_BUFFER_OBJECT_TYPE_M4V4P4,
			DMK_UNIFORM_BUFFER_OBJECT_TYPE_M4L4L32,
		};

		class DMKUniformBufferObject {
		public:
			DMKUniformBufferObject() {}
			~DMKUniformBufferObject() {}
		};
	}
}

#endif // !_DYNAMIK_ADGR_UNIFORM_BUFFER_OBJECT_H
