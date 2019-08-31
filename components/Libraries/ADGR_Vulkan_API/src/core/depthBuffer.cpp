#include "vkafx.h"
#include "depthBuffer.h"

#include "functions/bufferFunctions.h"
#include "functions/textureFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void depthBufferManager::init() {
				VkFormat depthFormat = findDepthFormat(physicalDevice);

				DMKCreateImageInfo info;
				info.device = *m_device;
				info.physicalDevice = physicalDevice;
				info.width = swapChainExtent.width;
				info.height = swapChainExtent.height;
				info.format = depthFormat;
				info.tiling = VK_IMAGE_TILING_OPTIMAL;
				info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				info.image = m_depthImage;
				info.imageMemory = m_depthImageMemory;
				info.mipLevels = 1;
				info.numSamples = msaaSamples;

				createImage(info);

				DMKCreateImageViewInfo viewInfo;
				viewInfo.device = device;
				viewInfo.image = *m_depthImage;
				viewInfo.format = depthFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
				viewInfo.mipLevels = 1;

				*m_depthImageView = createImageView(viewInfo);

				DMKTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = *m_device;
				transitionInfo.commandPool = commandPool;
				transitionInfo.image = *m_depthImage;
				transitionInfo.format = depthFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.graphicsQueue = graphicsQueue;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				transitionImageLayout(transitionInfo);
			}

			void depthBufferManager::createBuffer(DMKDepthBufferCreateInfo info) {
				VkFormat depthFormat = findDepthFormat(physicalDevice);

				DMKCreateImageInfo cInfo;
				cInfo.device = *m_device;
				cInfo.physicalDevice = physicalDevice;
				cInfo.width = swapChainExtent.width;
				cInfo.height = swapChainExtent.height;
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
				viewInfo.device = device;
				viewInfo.image = *info.image;
				viewInfo.format = info.imageFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
				viewInfo.mipLevels = 1;

				*m_depthImageView = createImageView(viewInfo);

				DMKTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = *m_device;
				transitionInfo.commandPool = commandPool;
				transitionInfo.image = *info.image;
				transitionInfo.format = info.imageFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.graphicsQueue = graphicsQueue;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				transitionImageLayout(transitionInfo);
			}

			void depthBufferManager::clear() {
				vkDestroyImageView(*m_device, *m_depthImageView, nullptr);
				vkDestroyImage(*m_device, *m_depthImage, nullptr);
				vkFreeMemory(*m_device, *m_depthImageMemory, nullptr);
			}
		}
	}
}
