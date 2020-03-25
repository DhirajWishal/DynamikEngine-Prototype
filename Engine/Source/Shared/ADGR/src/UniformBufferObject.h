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
	}
}

#endif // !_DYNAMIK_ADGR_UNIFORM_BUFFER_OBJECT_H
