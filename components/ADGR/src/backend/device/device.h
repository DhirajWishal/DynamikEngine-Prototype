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

			const std::vector<const char*> deviceExtensions = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};

			bool checkDeviceExtensionSupport(VkPhysicalDevice device);

			class device {
			public:
				device(VkDevice* device, VkPhysicalDevice* physicalDevice, VkSurfaceKHR* surface);
				~device() {}

				void initLogicalDevice(VkSurfaceKHR surface,
					VkQueue* graphicsQueue, VkQueue* presentQueue);
				void pickPhysicalDevice(VkInstance instance);
				bool isDeviceSuitable(VkPhysicalDevice device);
				int rateDeviceSuitability();

			private:
				VkDevice* myDevice;
				VkPhysicalDevice* myPhysicalDevice;
				VkSurfaceKHR* mySurface;
			};
		}
	}
}
