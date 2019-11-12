#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

namespace Dynamik {
	namespace ADGR {
		namespace core {
#define log(x) std::cout << x << std::endl
			void populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

			class debugger {
			public:
				debugger(VkInstance* instance, VkDebugUtilsMessengerEXT* debugMessenger);
				~debugger() {}

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
