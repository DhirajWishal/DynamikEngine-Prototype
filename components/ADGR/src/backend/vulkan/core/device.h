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

				VkDevice getDeviceCpy() { return *m_device; }
				VkDevice* getDeviceRef() { return m_device; }

				VkQueue* getPresentQueue() { return &presentQueue; }
				VkQueue* getGraphicsQueue() { return m_graphicsQueue; }

			private:
				void initPhysicalDevice(ADGRVulkanDataContainer* container) override;
				void initLogicalDevice(ADGRVulkanDataContainer* container) override;

				bool checkDeviceExtensionSupport(VkPhysicalDevice device);
				bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR* surface);

				VkInstance* m_instance = myInstance;
				VkDevice* m_device = myDevice;
				VkPhysicalDevice* m_physicalDevice = &physicalDevice;
				VkSurfaceKHR* m_surface = &surface;
				VkQueue* m_graphicsQueue = &graphicsQueue;
				VkSampleCountFlagBits* m_msaaSamples = &msaaSamples;

				const std::vector<const char*> deviceExtensions = {
					VK_KHR_SWAPCHAIN_EXTENSION_NAME
				};
			};
		}
	}
}
