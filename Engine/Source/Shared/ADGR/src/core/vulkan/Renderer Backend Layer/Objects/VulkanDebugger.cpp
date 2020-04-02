#include "adgrafx.h"
#include "VulkanDebugger.h"
#include "VulkanValidator.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData
			) {
				std::string myMessageStatement = "Vulkan Validation Layer ";
				std::string myMessagePreStatement = ": ";

				if (messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
					myMessagePreStatement = "(General): ";
				else if (messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
					myMessagePreStatement = "(Validation): ";
				else if (messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
					myMessagePreStatement = "(Performance): ";

				switch (messageSeverity) {
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
					std::cerr << myMessageStatement << myMessagePreStatement << pCallbackData->pMessage << std::endl;
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
					DMK_CORE_INFO("" + (myMessageStatement + myMessagePreStatement + std::string(pCallbackData->pMessage)));
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
					DMK_CORE_WARN("" + (myMessageStatement + myMessagePreStatement + std::string(pCallbackData->pMessage)));
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
					DMK_CORE_ERROR("" + (myMessageStatement + myMessagePreStatement + std::string(pCallbackData->pMessage)));
					break;
				default:
					std::cerr << myMessageStatement << myMessagePreStatement << pCallbackData->pMessage << std::endl;
					break;
				}

				return VK_FALSE;
			}

			VkResult VulkanDebugger::createDebugUtilsMessengerEXT(
				const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
				const VkAllocationCallbacks* pAllocator,
				VkDebugUtilsMessengerEXT* pDebugMessenger
			) {
				auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(myInstance.instance,
					"vkCreateDebugUtilsMessengerEXT");

				if (func != nullptr)
					return func(myInstance.instance, pCreateInfo, pAllocator, pDebugMessenger);
				else
					return VK_ERROR_EXTENSION_NOT_PRESENT;
			}

			void VulkanDebugger::destroyDebugUtilsMessengerEXT(
				const VkAllocationCallbacks* pAllocator
			) {
				auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(myInstance.instance,
					"vkDestroyDebugUtilsMessengerEXT");

				if (func != nullptr)
					func(myInstance.instance, myDebugMessenger, pAllocator);
			}

			void VulkanDebugger::populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
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

			void VulkanDebugger::initialize(VulkanInstance instance)
			{
				myInstance = instance;

				if (!VulkanValidator::enableValidationLayers)
					return;

				VkDebugUtilsMessengerCreateInfoEXT createInfo;
				populateDebugMessegerCreateInfo(createInfo);

				if (createDebugUtilsMessengerEXT(&createInfo, nullptr, &myDebugMessenger) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to set up debug messenger!");
			}

			void VulkanDebugger::terminate()
			{
				if (VulkanValidator::enableValidationLayers)
					destroyDebugUtilsMessengerEXT(nullptr);
			}
		}
	}
}