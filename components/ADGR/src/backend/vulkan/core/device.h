#pragma once

#include "core/Device.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class device : public Device {
			public:
				device() {}
				~device() {}

				void init(ADGRVulkanDataContainer* container) override;

			private:
				void initPhysicalDevice(ADGRVulkanDataContainer* container) override;
				void initLogicalDevice(ADGRVulkanDataContainer* container) override;

				bool checkDeviceExtensionSupport(VkPhysicalDevice device);
				bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR* surface);

				const std::vector<const char*> deviceExtensions = {
					VK_KHR_SWAPCHAIN_EXTENSION_NAME
				};
			};
		}
	}
}
