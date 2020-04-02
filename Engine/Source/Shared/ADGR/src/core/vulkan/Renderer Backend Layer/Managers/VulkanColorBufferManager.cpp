#include "adgrafx.h"
#include "VulkanColorBufferManager.h"

#include "VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			ADGRVulkanColorBufferContainer VulkanColorBufferManager::initialize(POINTER<VulkanCoreObject> core)
			{
				ADGRVulkanColorBufferContainer _container;
				VkFormat colorFormat = core->swapChainImageFormat;

				ADGRCreateImageInfo cinfo;
				cinfo.width = core->swapChainExtent.width;
				cinfo.height = core->swapChainExtent.height;
				cinfo.format = colorFormat;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &_container.image;
				cinfo.imageMemory = &_container.imageMemory;
				cinfo.mipLevels = 1;
				cinfo.numSamples = core->msaaSamples;
				cinfo.flags = NULL;

				VulkanFunctions::createImage(core, cinfo);

				ADGRCreateImageViewInfo viewInfo;
				viewInfo.image = _container.image;
				viewInfo.format = colorFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.mipLevels = 1;

				_container.imageView = VulkanFunctions::createImageView(core, viewInfo);

				ADGRTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = _container.image;
				transitionInfo.format = colorFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				transitionInfo.layerCount = 1;

				VulkanFunctions::transitionImageLayout(core, transitionInfo);

				return _container;
			}

			void VulkanColorBufferManager::terminate(POINTER<VulkanCoreObject> core, ADGRVulkanColorBufferContainer container)
			{
				vkDestroyImageView(core->logicalDevice, container.imageView, nullptr);
				vkDestroyImage(core->logicalDevice, container.image, nullptr);
				vkFreeMemory(core->logicalDevice, container.imageMemory, nullptr);
			}
		}
	}
}