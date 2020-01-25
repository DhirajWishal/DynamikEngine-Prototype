#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_INSTANCE_H
#define _DYNAMIK_ADGR_VULKAN_INSTANCE_H

#include "backend/vulkan/core/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class instanceManager  {
			public:
				instanceManager() {}
				~instanceManager() {}

				void init(ADGRVulkanDataContainer* container);

				void clear(ADGRVulkanDataContainer* container);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_INSTANCE_H
