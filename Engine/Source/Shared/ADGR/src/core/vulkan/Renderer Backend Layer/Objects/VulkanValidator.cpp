#include "adgrafx.h"
#include "VulkanValidator.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			B1 VulkanValidator::checkValidationLayerSupport()
			{
				UI32 layerCount = 0;
				vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

				ARRAY<VkLayerProperties> availableLayers(layerCount);
				vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

				for (const char* layerName : validationLayer) {
					B1 layerFound = false;

					for (auto layerProperties : availableLayers) {
						if (strcmp(layerName, layerProperties.layerName) == 0) {
							layerFound = true;
							break;
						}
					}

					if (!layerFound)
						return false;
				}

				return true;
			}

			ARRAY<const char*> VulkanValidator::validationLayer = {
				"VK_LAYER_KHRONOS_validation",
				//"VK_LAYER_LUNARG_api_dump",
				//"VK_LAYER_LUNARG_screenshot"
			};
		}
	}
}