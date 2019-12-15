#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_UNIFORM_H
#define _DYNAMIK_ADGR_VULKAN_UNIFORM_H

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct UniformBufferObject {
				alignas(16) glm::mat4 model;
				glm::mat4 view;
				glm::mat4 proj;
				glm::vec3 color;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_UNIFORM_H
