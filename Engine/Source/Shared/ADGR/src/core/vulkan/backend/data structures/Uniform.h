#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_UNIFORM_H
#define _DYNAMIK_ADGR_VULKAN_UNIFORM_H
#if defined(DMK_USE_VULKAN)

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

#endif
#endif // !_DYNAMIK_ADGR_VULKAN_UNIFORM_H
