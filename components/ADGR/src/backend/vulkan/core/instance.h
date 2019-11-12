#pragma once

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
