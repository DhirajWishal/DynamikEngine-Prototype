#include "adgrafx.h"
#include "VulkanDepthBufferManager.h"

#include "VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			ADGRVulkanDepthBufferContainer VulkanDepthBufferManager::initialize(POINTER<VulkanCoreObject> core)
			{
				ADGRVulkanDepthBufferContainer _container;
				VkFormat depthFormat = VulkanFunctions::findDepthFormat(core->physicalDevice);

				ADGRCreateImageInfo cinfo;
				cinfo.width = core->swapChainExtent.width;
				cinfo.height = core->swapChainExtent.height;
				cinfo.format = depthFormat;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &_container.image;
				cinfo.imageMemory = &_container.imageMemory;
				cinfo.mipLevels = 1;
				cinfo.numSamples = core->msaaSamples;
				cinfo.flags = NULL;

				VulkanFunctions::createImage(core, cinfo);

				ADGRCreateImageViewInfo viewInfo;
				viewInfo.image = _container.image;
				viewInfo.format = depthFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
				viewInfo.mipLevels = 1;

				_container.imageView = VulkanFunctions::createImageView(core, viewInfo);

				ADGRTransitionImageLayoutInfo transitionInfo;
				transitionInfo.image = _container.image;
				transitionInfo.format = depthFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				transitionInfo.layerCount = 2;

				VulkanFunctions::transitionImageLayout(core, transitionInfo);

				return _container;
			}

			void VulkanDepthBufferManager::terminate(POINTER<VulkanCoreObject> core, ADGRVulkanDepthBufferContainer container)
			{
				vkDestroyImageView(core->logicalDevice, container.imageView, nullptr);
				vkDestroyImage(core->logicalDevice, container.image, nullptr);
				vkFreeMemory(core->logicalDevice, container.imageMemory, nullptr);
			}
		}
	}
}