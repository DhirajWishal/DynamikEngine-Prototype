#include "adgrafx.h"
#include "depthBufferManager.h"

#include "functions/bufferFunctions.h"
#include "functions/textureFunctions.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void depthBufferManager::init(ADGRVulkanDataContainer* container) {
				VkFormat depthFormat = findDepthFormat(container->physicalDevice);

				DMKCreateImageInfo info;
				info.device = container->device;
				info.physicalDevice = container->physicalDevice;
				info.width = container->swapchainContainer.swapchainExtent.width;
				info.height = container->swapchainContainer.swapchainExtent.height;
				info.format = depthFormat;
				info.tiling = VK_IMAGE_TILING_OPTIMAL;
				info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				info.image = &container->depthBufferContainer.image;
				info.imageMemory = &container->depthBufferContainer.imageMemory;
				info.mipLevels = 1;
				info.numSamples = container->msaaSamples;
				info.flags = NULL;

				createImage(info);

				DMKCreateImageViewInfo viewInfo;
				viewInfo.device = container->device;
				viewInfo.image = container->depthBufferContainer.image;
				viewInfo.format = depthFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
				viewInfo.mipLevels = 1;

				container->depthBufferContainer.imageView = createImageView(viewInfo);

				DMKTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = container->device;
				transitionInfo.commandPool = container->commandBufferContainer.commandPool;
				transitionInfo.image = container->depthBufferContainer.image;
				transitionInfo.format = depthFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.graphicsQueue = container->graphicsQueue;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				transitionInfo.layerCount = 1;

				transitionImageLayout(transitionInfo);
			}

			void depthBufferManager::createBuffer(ADGRVulkanDataContainer* container, DMKDepthBufferCreateInfo info) {
				VkFormat depthFormat = findDepthFormat(container->physicalDevice);

				DMKCreateImageInfo cInfo;
				cInfo.device = container->device;
				cInfo.physicalDevice = container->physicalDevice;
				cInfo.width = container->swapchainContainer.swapchainExtent.width;
				cInfo.height = container->swapchainContainer.swapchainExtent.height;
				cInfo.format = info.imageFormat;
				cInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				cInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cInfo.image = info.image;
				cInfo.imageMemory = info.imageMemory;
				cInfo.mipLevels = 1;
				cInfo.numSamples = info.msaaSamples;

				createImage(cInfo);

				DMKCreateImageViewInfo viewInfo;
				viewInfo.device = container->device;
				viewInfo.image = *info.image;
				viewInfo.format = info.imageFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
				viewInfo.mipLevels = 1;

				container->depthBufferContainer.imageView = createImageView(viewInfo);

				DMKTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = container->device;
				transitionInfo.commandPool = container->commandBufferContainer.commandPool;
				transitionInfo.image = *info.image;
				transitionInfo.format = info.imageFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.graphicsQueue = container->graphicsQueue;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				transitionImageLayout(transitionInfo);
			}

			void depthBufferManager::clear(ADGRVulkanDataContainer* container) {
				vkDestroyImageView(container->device, container->depthBufferContainer.imageView, nullptr);
				vkDestroyImage(container->device, container->depthBufferContainer.image, nullptr);
				vkFreeMemory(container->device, container->depthBufferContainer.imageMemory, nullptr);
			}
		}
	}
}
#endif // DMK_USE_VULKAN
