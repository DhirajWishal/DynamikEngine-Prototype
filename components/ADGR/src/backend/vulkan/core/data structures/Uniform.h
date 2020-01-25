#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_UNIFORM_H
#define _DYNAMIK_ADGR_VULKAN_UNIFORM_H

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct UniformBufferObject {
				alignas(16) glm::mat4 model;
				alignas(16) glm::mat4 view;
				alignas(16) glm::mat4 proj;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_UNIFORM_H
