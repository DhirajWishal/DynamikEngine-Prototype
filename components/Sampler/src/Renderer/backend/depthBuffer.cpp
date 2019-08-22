#include "smpafx.h"
#include "depthBuffer.h"

#include "functions/bufferFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			void depthBufferManager::init() {
				VkFormat depthFormat = findDepthFormat(physicalDevice);

				createImage(*m_device, *myPhysicalDevice, swapChainExtent.width, swapChainExtent.height,
					depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *myDepthImage, *myDepthImageMemory, 1,
					msaaSamples);
				*myDepthImageView = createImageView(*m_device, *myDepthImage, depthFormat,
					VK_IMAGE_ASPECT_DEPTH_BIT, 1);

				transitionImageLayout(*m_device, commandPool, *myDepthImage, depthFormat,
					VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, graphicsQueue,
					1);
			}

			void depthBufferManager::createBuffer(DMKDepthBufferCreateInfo info) {
				VkFormat depthFormat = findDepthFormat(physicalDevice);

				createImage(*m_device, physicalDevice, swapChainExtent.width, swapChainExtent.height,
					info.imageFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, info.image, info.imageMemory, 1,
					info.msaaSamples);
				info.imageViews = createImageView(*m_device, info.image, info.imageFormat,
					VK_IMAGE_ASPECT_DEPTH_BIT, 1);

				transitionImageLayout(*m_device, commandPool, info.image, info.imageFormat,
					VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, graphicsQueue,
					1);
			}
		}
	}
}
