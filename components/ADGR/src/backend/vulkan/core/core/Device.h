#pragma once

#include "Core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class Device : public Core {
			public:
				Device() {}
				virtual ~Device() {}

				virtual void initPhysicalDevice(ADGRVulkanDataContainer* container) {}
				virtual void initLogicalDevice(ADGRVulkanDataContainer* container) {}

			protected:
			};
		}
	}
}
