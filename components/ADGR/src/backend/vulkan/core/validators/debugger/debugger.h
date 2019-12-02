#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_ADGR_VULKAN_DEBUGGER_H
#define _DYNAMIK_ADGR_VULKAN_DEBUGGER_H

#include "backend/vulkan/core/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
#define log(x) std::cout << x << std::endl
			void populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

			class debugger {
			public:
				debugger() {}
				debugger(VkInstance* instance, VkDebugUtilsMessengerEXT* debugMessenger);
				~debugger() {}

				void initDebugger(ADGRVulkanDataContainer* container);

				void setupDebugMessenger();

				VkResult createDebugUtilsMessengerEXT(
					const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
					const VkAllocationCallbacks* pAllocator,
					VkDebugUtilsMessengerEXT* pDebugMessenger
				);

				void destroyDebugUtilsMessengerEXT(
					const VkAllocationCallbacks* pAllocator
				);

				void populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

				static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
					VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
					VkDebugUtilsMessageTypeFlagsEXT messageType,
					const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
					void* pUserData
				);

				void deleteDebugger();

			private:
				VkInstance* myInstance;
				VkDebugUtilsMessengerEXT* myDebugMessenger;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_DEBUGGER_H
