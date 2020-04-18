#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_ONE_TIME_COMPUTE_H
#define _DYNAMIK_ADGR_VULKAN_ONE_TIME_COMPUTE_H

#include "VulkanComputeManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanOneTimeCompute {
			public:
				VulkanOneTimeCompute();
				virtual ~VulkanOneTimeCompute();
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_ONE_TIME_COMPUTE_H
