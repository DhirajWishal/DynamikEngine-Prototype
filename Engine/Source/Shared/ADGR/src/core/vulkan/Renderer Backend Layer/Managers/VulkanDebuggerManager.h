#pragma once
#ifndef _DYNAMIK_VADGR_VULKAN_DEBUGGER_MANAGER_H
#define _DYNAMIK_VADGR_VULKAN_DEBUGGER_MANAGER_H

#include "Objects/VulkanCoreObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanDebuggerManager {
			public:
				VulkanDebuggerManager() {}
				VulkanDebuggerManager(POINTER<VulkanCoreObject> core) : myInstance(core->instance) {}
				~VulkanDebuggerManager() {}

				void initialize(POINTER<VulkanCoreObject> core);
				void terminate(POINTER<VulkanCoreObject> core);

				static void populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

			private:
				VkResult createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
				void destroyDebugUtilsMessengerEXT(POINTER<VulkanCoreObject> core, const VkAllocationCallbacks* pAllocator);

				VkInstance myInstance;
			};
		}
	}
}

#endif // !_DYNAMIK_VADGR_VULKAN_DEBUGGER_H
