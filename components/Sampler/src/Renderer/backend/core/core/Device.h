#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_DEVICE_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_DEVICE_H

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

#endif	// !_DYNAMIK_ADGR_VULKAN_RENDERER_DEVICE_H
