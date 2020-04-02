#pragma once
#ifndef _DYNAMIK_VADGR_VULKAN_DEBUGGER_H
#define _DYNAMIK_VADGR_VULKAN_DEBUGGER_H

#include "VulkanInstance.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanDebugger {
			public:
				VulkanDebugger() {}
				VulkanDebugger(VulkanInstance instance) : myInstance(instance) {}
				~VulkanDebugger() {}

				void initialize(VulkanInstance instance);
				void terminate();

				VkDebugUtilsMessengerEXT myDebugMessenger = VK_NULL_HANDLE;

				static void populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

			private:
				VkResult createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
				void destroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);

				VulkanInstance myInstance;
			};
		}
	}
}

#endif // !_DYNAMIK_VADGR_VULKAN_DEBUGGER_H
