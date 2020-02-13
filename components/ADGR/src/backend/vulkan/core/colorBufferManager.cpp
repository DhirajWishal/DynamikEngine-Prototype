#include "adgrafx.h"

#include "colorBufferManager.h"

#include "data structures/vulkan.h"
#include "functions/textureFunctions.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void colorBufferManager::createBuffer(VkBuffer* buffer, size_t size,
				VkBufferUsageFlags flags, VkBufferUsageFlagBits memoryFlags) {
			}

			void colorBufferManager::initResources(ADGRVulkanDataContainer* container) {
				VkFormat colorFormat = container->swapchainContainer.swapchainImageFormat;

				DMKCreateImageInfo info;
				info.device = container->device;
				info.physicalDevice = container->physicalDevice;
				info.width = container->swapchainContainer.swapchainExtent.width;
				info.height = container->swapchainContainer.swapchainExtent.height;
				info.format = colorFormat;
				info.tiling = VK_IMAGE_TILING_OPTIMAL;
				info.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				info.image = &container->colorBufferContainer.image;
				info.imageMemory = &container->colorBufferContainer.imageMemory;
				info.mipLevels = 1;
				info.numSamples = container->msaaSamples;
				info.flags = NULL;

				createImage(info);

				DMKCreateImageViewInfo viewInfo;
				viewInfo.device = container->device;
				viewInfo.image = container->colorBufferContainer.image;
				viewInfo.format = colorFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.mipLevels = 1;

				container->colorBufferContainer.imageView = createImageView(viewInfo);

				DMKTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = container->device;
				transitionInfo.commandPool = container->commandBufferContainer.commandPool;
				transitionInfo.image = container->colorBufferContainer.image;
				transitionInfo.format = colorFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.graphicsQueue = container->graphicsQueue;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				transitionInfo.layerCount = 1;

				transitionImageLayout(transitionInfo);
			}

			void colorBufferManager::createResources(ADGRVulkanDataContainer* container, DMKColorImageCreateInfo Cinfo) {
				VkFormat colorFormat = container->swapchainContainer.swapchainImageFormat;

				DMKCreateImageInfo info;
				info.device = container->device;
				info.physicalDevice = container->physicalDevice;
				info.width = container->swapchainContainer.swapchainExtent.width;
				info.height = container->swapchainContainer.swapchainExtent.height;
				info.format = Cinfo.colorFormat;
				info.tiling = VK_IMAGE_TILING_OPTIMAL;
				info.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				info.image = &Cinfo.image;
				info.imageMemory = &Cinfo.imageMemory;
				info.mipLevels = 1;
				info.numSamples = Cinfo.msaaSamples;

				createImage(info);

				DMKCreateImageViewInfo viewInfo;
				viewInfo.device = container->device;
				viewInfo.image = Cinfo.image;
				viewInfo.format = Cinfo.colorFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.mipLevels = 1;

				container->colorBufferContainer.imageView = createImageView(viewInfo);

				DMKTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = container->device;
				transitionInfo.commandPool = container->commandBufferContainer.commandPool;
				transitionInfo.image = Cinfo.image;
				transitionInfo.format = Cinfo.colorFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.graphicsQueue = container->graphicsQueue;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				transitionImageLayout(transitionInfo);
			}

			void colorBufferManager::clear(ADGRVulkanDataContainer* container) {
				vkDestroyImageView(container->device, container->colorBufferContainer.imageView, nullptr);
				vkDestroyImage(container->device, container->colorBufferContainer.image, nullptr);
				vkFreeMemory(container->device, container->colorBufferContainer.imageMemory, nullptr);
			}
		}
	}
}
#endif // DMK_USE_VULKAN
