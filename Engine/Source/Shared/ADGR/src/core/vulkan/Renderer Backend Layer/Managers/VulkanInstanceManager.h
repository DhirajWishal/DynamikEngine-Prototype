#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_INSTANCE_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_INSTANCE_MANAGER_H

#include <string>
#include "Objects/VulkanCoreObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanInstanceInitInfo {
				std::string applicationName = "Dynamik Engine";
				std::string engineName = "Dynamik";
			};

			class VulkanInstanceManager {
			public:
				VulkanInstanceManager() {}
				~VulkanInstanceManager() {}

				void initialize(POINTER<VulkanCoreObject> core, ADGRVulkanInstanceInitInfo info);
				void terminate(POINTER<VulkanCoreObject> core);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_INSTANCE_H
