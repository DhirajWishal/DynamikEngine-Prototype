#include "adgrafx.h"
#include "VulkanColorBuffer.h"

#include "VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanColorBuffer::initialize(ADGRVulkanColorBufferInitInfo info)
			{
				VkFormat colorFormat = info.swapChain.mySwapChainImageFormat;

				ADGRCreateImageInfo cinfo;
				cinfo.device = info.device;
				cinfo.width = info.swapChain.mySwapChainExtent.width;
				cinfo.height = info.swapChain.mySwapChainExtent.height;
				cinfo.format = colorFormat;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &image;
				cinfo.imageMemory = &imageMemory;
				cinfo.mipLevels = 1;
				cinfo.numSamples = info.global->msaaSamples;
				cinfo.flags = NULL;

				VulkanFunctions::createImage(cinfo);

				ADGRCreateImageViewInfo viewInfo;
				viewInfo.device = info.device;
				viewInfo.image = image;
				viewInfo.format = colorFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.mipLevels = 1;

				imageView = VulkanFunctions::createImageView(viewInfo);

				ADGRTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = info.device;
				transitionInfo.commandBuffer = info.commandBuffer;
				transitionInfo.image = image;
				transitionInfo.format = colorFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.queue = info.queue;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				transitionInfo.layerCount = 1;

				VulkanFunctions::transitionImageLayout(transitionInfo);
			}

			void VulkanColorBuffer::terminate(VulkanDevice device)
			{
				vkDestroyImageView(device.logicalDevice, imageView, nullptr);
				vkDestroyImage(device.logicalDevice, image, nullptr);
				vkFreeMemory(device.logicalDevice, imageMemory, nullptr);
			}
		}
	}
}