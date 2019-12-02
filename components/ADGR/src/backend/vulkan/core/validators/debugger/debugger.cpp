/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "debugger.h"
#include "backend/vulkan/core/validators/validators.h"

#include "backend/defines.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			void populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
				createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
				createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
					| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
					| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
				createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
					| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
					| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
				createInfo.pfnUserCallback = debugger::debugCallback;
			}

			debugger::debugger(VkInstance* instance, VkDebugUtilsMessengerEXT* debugMessenger) :
				myInstance(instance), myDebugMessenger(debugMessenger) {
			}

			VKAPI_ATTR VkBool32 VKAPI_CALL debugger::debugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData
			) {
				std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

				return VK_FALSE;
			}

			VkResult debugger::createDebugUtilsMessengerEXT(
				const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
				const VkAllocationCallbacks* pAllocator,
				VkDebugUtilsMessengerEXT* pDebugMessenger
			) {
				auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*myInstance,
					"vkCreateDebugUtilsMessengerEXT");

				if (func != nullptr)
					return func(*myInstance, pCreateInfo, pAllocator, pDebugMessenger);
				else
					return VK_ERROR_EXTENSION_NOT_PRESENT;
			}

			void debugger::destroyDebugUtilsMessengerEXT(
				const VkAllocationCallbacks* pAllocator
			) {
				auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*myInstance,
					"vkDestroyDebugUtilsMessengerEXT");

				if (func != nullptr)
					func(*myInstance, *myDebugMessenger, pAllocator);
			}

			void debugger::populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
				createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
				createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
					| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
					| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
				createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
					| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
					| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
				createInfo.pfnUserCallback = debugCallback;
			}

			void debugger::initDebugger(ADGRVulkanDataContainer* container) {
				myInstance = &container->instance;
				myDebugMessenger = &container->debugMessenger;
			}

			void debugger::setupDebugMessenger() {
				if (!enableValidationLayers)
					return;

				VkDebugUtilsMessengerCreateInfoEXT createInfo;
				populateDebugMessegerCreateInfo(createInfo);

				if (createDebugUtilsMessengerEXT(&createInfo, nullptr, myDebugMessenger) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to set up debug messenger!");
			}

			void debugger::deleteDebugger() {
				if (enableValidationLayers)
					destroyDebugUtilsMessengerEXT(nullptr);
			}
		}
	}
}