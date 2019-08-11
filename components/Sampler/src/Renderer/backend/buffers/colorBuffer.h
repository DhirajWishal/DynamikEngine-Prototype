#pragma once

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class colorBuffer {
			public:
				colorBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkImage* image, VkDeviceMemory* deviceMemeory,
					VkImageView* imageView);
				~colorBuffer() {}

				void initResources(VkFormat swapChainImageFormat, VkExtent2D swapChainExtent,
					VkSampleCountFlagBits msaaSamples, VkCommandPool commandPool, VkQueue graphicsQueue);
				void deleteBuffer();

			private:
				VkDevice* myDevice;
				VkPhysicalDevice* myPhysicalDevice;
				VkImage* myColorImage;
				VkDeviceMemory* myColorImageMemory;
				VkImageView* myColorImageView;

			};
		}
	}
}
