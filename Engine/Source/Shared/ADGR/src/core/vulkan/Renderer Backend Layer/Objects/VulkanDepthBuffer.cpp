#include "adgrafx.h"
#include "VulkanDepthBuffer.h"

#include "VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanDepthBuffer::initialize(ADGRVulkanDepthBufferInitInfo info)
			{
				VkFormat depthFormat = VulkanFunctions::findDepthFormat(info.device.physicalDevice);

				ADGRCreateImageInfo cInfo;
				cInfo.device = info.device;
				cInfo.width = info.swapChain.mySwapChainExtent.width;
				cInfo.height = info.swapChain.mySwapChainExtent.height;
				cInfo.format = depthFormat;
				cInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				cInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cInfo.image = &image;
				cInfo.imageMemory = &imageMemory;
				cInfo.mipLevels = 1;
				cInfo.numSamples = info.global->msaaSamples;
				cInfo.flags = NULL;

				VulkanFunctions::createImage(cInfo);

				ADGRCreateImageViewInfo viewInfo;
				viewInfo.device = info.device;
				viewInfo.image = image;
				viewInfo.format = depthFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
				viewInfo.mipLevels = 1;

				imageView = VulkanFunctions::createImageView(viewInfo);

				ADGRTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = info.device;
				transitionInfo.commandBuffer = info.commandBuffer;
				transitionInfo.image = image;
				transitionInfo.format = depthFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.queue = info.queue;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				transitionInfo.layerCount = 2;

				VulkanFunctions::transitionImageLayout(transitionInfo);
			}

			void VulkanDepthBuffer::terminate(VulkanDevice device)
			{
				vkDestroyImageView(device.logicalDevice, imageView, nullptr);
				vkDestroyImage(device.logicalDevice, image, nullptr);
				vkFreeMemory(device.logicalDevice, imageMemory, nullptr);
			}
		}
	}
}