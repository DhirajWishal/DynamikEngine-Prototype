#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_INSTANCE_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_INSTANCE_MANAGER_H
#ifdef DMK_USE_VULKAN

#include "backend/vulkan/core/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class instanceManager {
			public:
				instanceManager() {}
				~instanceManager() {}

				void init(ADGRVulkanDataContainer* container);

				void clear(ADGRVulkanDataContainer* container);
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_INSTANCE_H
