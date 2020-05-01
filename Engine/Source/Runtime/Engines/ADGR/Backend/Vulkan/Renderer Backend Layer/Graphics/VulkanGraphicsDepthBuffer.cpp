#include "dmkafx.h"
#include "VulkanGraphicsDepthBuffer.h"

#include "../Common/VulkanUtilities.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanGraphicsDepthBuffer::initialize(
				VkDevice logicalDevice,
				VkPhysicalDevice physicalDevice,
				VkCommandPool commandPool,
				VkQueue graphicsQueue,
				VkQueue presentQueue,
				VkExtent2D swapChainExtent,
				VkSampleCountFlagBits msaaSamples)
			{
				VkFormat depthFormat = VulkanUtilities::findDepthFormat(physicalDevice);

				ADGRVulkanCreateImageInfo cinfo;
				cinfo.width = swapChainExtent.width;
				cinfo.height = swapChainExtent.height;
				cinfo.format = depthFormat;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &image;
				cinfo.imageMemory = &imageMemory;
				cinfo.mipLevels = 1;
				cinfo.numSamples = msaaSamples;
				cinfo.flags = NULL;

				VulkanUtilities::createImage(logicalDevice, physicalDevice, cinfo);

				ADGRVulkanCreateImageViewInfo viewInfo;
				viewInfo.image = image;
				viewInfo.format = depthFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
				viewInfo.mipLevels = 1;

				imageView = VulkanUtilities::createImageView(logicalDevice, viewInfo);

				ADGRVulkanTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = image;
				transitionInfo.format = depthFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				transitionInfo.layerCount = 2;

				VulkanUtilities::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);
			}

			void VulkanGraphicsDepthBuffer::terminate(VkDevice logicalDevice)
			{
				vkDestroyImageView(logicalDevice, imageView, nullptr);
				vkDestroyImage(logicalDevice, image, nullptr);
				vkFreeMemory(logicalDevice, imageMemory, nullptr);
			}
		}
	}
}