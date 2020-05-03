#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_ONE_TIME_COMPUTE_H
#define _DYNAMIK_RENDERER_VULKAN_ONE_TIME_COMPUTE_H

#include "VulkanComputeManager.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			class VulkanOneTimeCompute {
			public:
				VulkanOneTimeCompute();
				virtual ~VulkanOneTimeCompute();
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_ONE_TIME_COMPUTE_H
