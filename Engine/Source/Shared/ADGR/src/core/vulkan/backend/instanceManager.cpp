#include "adgrafx.h"
#include "instanceManager.h"

#include "validators/validators.h"
#include "extensions/extensions.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		namespace core {
			void instanceManager::init(ADGRVulkanDataContainer* container) {
				if (enableValidationLayers && !checkValidationLayerSupport())
					DMK_CORE_FATAL("validation layers requested, but not available!");

				// instance info
				VkApplicationInfo appInfo = {};
				appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				appInfo.pApplicationName = "Dynamik Engine";
				appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
				appInfo.pEngineName = "Dynamik";
				appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
				appInfo.apiVersion = VK_API_VERSION_1_1;

				VkInstanceCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				createInfo.pApplicationInfo = &appInfo;

				auto extentions = getRequiredExtentions(enableValidationLayers);
				createInfo.enabledExtensionCount = static_cast<UI32>(extentions.size());
				createInfo.ppEnabledExtensionNames = extentions.data();

				VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
				if (enableValidationLayers) {
					createInfo.enabledLayerCount = static_cast<UI32>(validationLayer.size());
					createInfo.ppEnabledLayerNames = validationLayer.data();

					debugger _localDebugger = {};
					_localDebugger.populateDebugMessegerCreateInfo(debugCreateInfo);
					createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
				}
				else {
					createInfo.enabledLayerCount = 0;

					createInfo.pNext = nullptr;
				}

				if (vkCreateInstance(&createInfo, nullptr, &container->instance) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to create instance!");
			}

			void instanceManager::clear(ADGRVulkanDataContainer* container) {
				vkDestroyInstance(container->instance, nullptr);
			}
		}
	}
}
#endif // DMK_USE_VULKAN