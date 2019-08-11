#include "smpafx.h"
#include "colorBuffer.h"
#include "Renderer/backend/texture/texture.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			colorBuffer::colorBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkImage* image,
				VkDeviceMemory* deviceMemeory,VkImageView* imageView) :
				myDevice(device), myPhysicalDevice(physicalDevice), myColorImage(image), myColorImageMemory(deviceMemeory),
				myColorImageView(imageView) {

			}

			void colorBuffer::initResources(VkFormat swapChainImageFormat, VkExtent2D swapChainExtent,
				VkSampleCountFlagBits msaaSamples, VkCommandPool commandPool, VkQueue graphicsQueue) {
				VkFormat colorFormat = swapChainImageFormat;

				createImage(*myDevice, *myPhysicalDevice, swapChainExtent.width, swapChainExtent.height,
					colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT 
					| VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					*myColorImage, *myColorImageMemory, 1, msaaSamples);

				*myColorImageView = createImageView(*myDevice, *myColorImage, colorFormat,
					VK_IMAGE_ASPECT_COLOR_BIT, 1);

				transitionImageLayout(*myDevice, commandPool, *myColorImage, colorFormat,
					VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, graphicsQueue, 1);
			}

			void colorBuffer::deleteBuffer() {
				vkDestroyImageView(*myDevice, *myColorImageView, nullptr);
				vkDestroyImage(*myDevice, *myColorImage, nullptr);
				vkFreeMemory(*myDevice, *myColorImageMemory, nullptr);
			}
		}
	}
}
