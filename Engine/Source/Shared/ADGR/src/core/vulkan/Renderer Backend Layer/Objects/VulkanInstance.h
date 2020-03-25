#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_INSTANCE_H
#define _DYNAMIK_ADGR_VULKAN_INSTANCE_H

#include <string>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanInstanceInitInfo {
				std::string applicationName = "Dynamik Engine";
				std::string engineName = "Dynamik";
			};

			class VulkanInstance {
			public:
				VulkanInstance() {}
				~VulkanInstance() {}

				void initialize(ADGRVulkanInstanceInitInfo info);
				void terminate();

				VkInstance instance = VK_NULL_HANDLE;
				VkSurfaceKHR surface = VK_NULL_HANDLE;
				VkSurfaceCapabilitiesKHR surfaceCapabilities;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_INSTANCE_H
