#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_OBJECTS_CONTAINER_H
#define _DYNAMIK_ADGR_VULKAN_OBJECTS_CONTAINER_H

#include "VulkanObject.h"

#include "VulkanDebugger.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanObjectsContainer {
			public:
				VulkanObjectsContainer() {}
				~VulkanObjectsContainer() {}

				VulkanInstance instance;
				VulkanDebugger debugger;
				VulkanDevice device;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_OBJECTS_CONTAINER_H
