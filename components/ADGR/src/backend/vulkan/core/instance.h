#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_INSTANCE_H
#define _DYNAMIK_ADGR_VULKAN_INSTANCE_H

#include "core/Instance.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class instanceManager : public Instance {
			public:
				instanceManager() {}
				~instanceManager() {}

				void init(ADGRVulkanDataContainer* container) override;

				void clear(ADGRVulkanDataContainer* container) override;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_INSTANCE_H
