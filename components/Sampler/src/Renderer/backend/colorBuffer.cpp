#include "smpafx.h"
#include "colorBuffer.h"

#include "data structures/vulkan.h"
#include "functions/textureFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void colorBufferManager::createBuffer(VkBuffer* buffer, size_t size,
				VkBufferUsageFlags flags, VkBufferUsageFlagBits memoryFlags) {
			}

			void colorBufferManager::initResources() {
				VkFormat colorFormat = swapChainImageFormat;

				DMKCreateImageInfo info;
				info.device = *m_device;
				info.physicalDevice = physicalDevice;
				info.width = swapChainExtent.width;
				info.height = swapChainExtent.height;
				info.format = colorFormat;
				info.tiling = VK_IMAGE_TILING_OPTIMAL;
				info.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				info.image = m_colorImage;
				info.imageMemory = m_colorImageMemory;
				info.mipLevels = 1;
				info.numSamples = msaaSamples;

				createImage(info);

				DMKCreateImageViewInfo viewInfo;
				viewInfo.device = device;
				viewInfo.image = *m_colorImage;
				viewInfo.format = colorFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.mipLevels = 1;

				*m_colorImageView = createImageView(viewInfo);

				DMKTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = *m_device;
				transitionInfo.commandPool = commandPool;
				transitionInfo.image = *m_colorImage;
				transitionInfo.format = colorFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.graphicsQueue = graphicsQueue;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				transitionImageLayout(transitionInfo);
			}

			void colorBufferManager::createResources(DMKColorImageCreateInfo Cinfo) {
				VkFormat colorFormat = swapChainImageFormat;

				DMKCreateImageInfo info;
				info.device = *m_device;
				info.physicalDevice = physicalDevice;
				info.width = swapChainExtent.width;
				info.height = swapChainExtent.height;
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
				viewInfo.device = device;
				viewInfo.image = Cinfo.image;
				viewInfo.format = Cinfo.colorFormat;
				viewInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.mipLevels = 1;

				*m_colorImageView = createImageView(viewInfo);

				DMKTransitionImageLayoutInfo transitionInfo;
				transitionInfo.device = *m_device;
				transitionInfo.commandPool = commandPool;
				transitionInfo.image = Cinfo.image;
				transitionInfo.format = Cinfo.colorFormat;
				transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transitionInfo.graphicsQueue = graphicsQueue;
				transitionInfo.mipLevels = 1;
				transitionInfo.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				transitionImageLayout(transitionInfo);
			}
		}
	}
}
