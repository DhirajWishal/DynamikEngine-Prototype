#include "adgrafx.h"
#include "VulkanGraphicsColorBuffer.h"

#include "VulkanGraphicsFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanGraphicsColorBuffer::initialize(
				VkDevice logicalDevice,
				VkPhysicalDevice physicalDevice,
				VkCommandPool commandPool,
				VkQueue graphicsQueue,
				VkQueue presentQueue,
				VkFormat swapChainImageFormat,
				VkExtent2D swapChainExtent,
				VkSampleCountFlagBits msaaSamples)
			{
				VkFormat colorFormat = swapChainImageFormat;

				ADGRVulkanCreateImageInfo cinfo;
				cinfo.width = swapChainExtent.width;
				cinfo.height = swapChainExtent.height;
				cinfo.format = colorFormat;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &image;
				cinfo.imageMemory = &imageMemory;
				cinfo.mipLevels = 1;
				cinfo.numSamples = msaaSamples;
				cinfo.flags = NULL;

				VulkanGraphicsFunctions::createImage(logicalDevice, physicalDevice, cinfo);

				ADGRVulkanCreateImageViewInfo viewInfo;
				viewInfo.image = image;
				viewInfo.format = colorFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.mipLevels = 1;

				imageView = VulkanGraphicsFunctions::createImageView(logicalDevice, viewInfo);

				ADGRVulkanTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = image;
				transitionInfo.format = colorFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				transitionInfo.layerCount = 1;

				VulkanGraphicsFunctions::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, presentQueue, transitionInfo);
			}

			void VulkanGraphicsColorBuffer::terminate(VkDevice logicalDevice)
			{
				vkDestroyImageView(logicalDevice, imageView, nullptr);
				vkDestroyImage(logicalDevice, image, nullptr);
				vkFreeMemory(logicalDevice, imageMemory, nullptr);
			}
		}
	}
}