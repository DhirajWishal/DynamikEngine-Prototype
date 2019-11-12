#pragma once

#include "Core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class Device : public Core {
			public:
				Device() {}
				virtual ~Device() {}

				virtual void initPhysicalDevice() {}
				virtual void initLogicalDevice() {}

			protected:
				VkInstance* myInstance = &instance;
				VkDevice* myDevice = &device;
				VkPhysicalDevice* myPhysicalDevice = &physicalDevice;
			};
		}
	}
}
