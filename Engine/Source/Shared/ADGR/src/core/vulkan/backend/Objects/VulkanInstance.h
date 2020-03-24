#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_INSTANCE_H
#define _DYNAMIK_ADGR_VULKAN_INSTANCE_H

#include "core/vulkan/backend/vulkanFormat.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanInstance {
			public:
				VulkanInstance() {}
				~VulkanInstance() {}

				void initialize();
				void terminate();

				VkInstance get() { return myInstance; }
				POINTER<VkInstance> getAddress() { return &myInstance; }

				VkSurfaceKHR getSurface() { return mySurface; }
				POINTER<VkSurfaceKHR> getSurfaceAddress() { return &mySurface; }

				VkSurfaceCapabilitiesKHR getSurfaceCapabilities() { return mySurfaceCapabilities; }

			private:
				VkInstance myInstance = VK_NULL_HANDLE;
				VkSurfaceKHR mySurface = VK_NULL_HANDLE;
				VkSurfaceCapabilitiesKHR mySurfaceCapabilities;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_INSTANCE_H
