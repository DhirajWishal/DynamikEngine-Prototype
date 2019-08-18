#include "adgrafx.h"
#include "depthBuffer.h"
#include "backend/texture/texture.h"



namespace Dynamik {
	namespace ADGR {
		namespace core {

			depthBuffer::depthBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
				VkImage* depthImage, VkDeviceMemory* depthImgMem, VkImageView* depthImgView) :
				myDevice(device), myPhysicalDevice(physicalDevice), myDepthImage(depthImage),
				myDepthImageMemory(depthImgMem), myDepthImageView(depthImgView) {

			}

			depthBuffer::~depthBuffer() {

			}

			void depthBuffer::initBuffer(VkExtent2D swapChainExtent, VkCommandPool commandPool,
				VkQueue graphicsQueue, uint32 mipLevels, VkSampleCountFlagBits msaaSamples) {
				VkFormat depthFormat = findDepthFormat();

				createImage(*myDevice, *myPhysicalDevice, swapChainExtent.width, swapChainExtent.height,
					depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *myDepthImage, *myDepthImageMemory, 1,
					msaaSamples);
				*myDepthImageView = createImageView(*myDevice, *myDepthImage, depthFormat,
					VK_IMAGE_ASPECT_DEPTH_BIT, 1);

				transitionImageLayout(*myDevice, commandPool, *myDepthImage, depthFormat,
					VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, graphicsQueue,
					1);
			}

			VkFormat depthBuffer::findSupportedFormat(const std::vector<VkFormat>& candidates,
				VkImageTiling tiling, VkFormatFeatureFlags features) {
				for (VkFormat format : candidates) {
					VkFormatProperties props;

					vkGetPhysicalDeviceFormatProperties(*myPhysicalDevice, format, &props);

					if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
						return format;
					else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
						return format;
				}

				std::runtime_error("failed to find supported format!");
			}

			void depthBuffer::deleteBuffer() {
				vkDestroyImageView(*myDevice, *myDepthImageView, nullptr);
				vkDestroyImage(*myDevice, *myDepthImage, nullptr);
				vkFreeMemory(*myDevice, *myDepthImageMemory, nullptr);
			}

			VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
				VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice) {
				for (VkFormat format : candidates) {
					VkFormatProperties props;

					vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

					if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
						return format;
					}
					else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
						return format;
					}
				}

				std::runtime_error("failed to find supported format!");
			}

			VkFormat depthBuffer::findDepthFormat() {
				return findSupportedFormat(
					{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
					VK_IMAGE_TILING_OPTIMAL,
					VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
				);
			}

			VkFormat findDepthFormat(VkPhysicalDevice physicalDevice) {
				return findSupportedFormat(
					{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
					VK_IMAGE_TILING_OPTIMAL,
					VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
					physicalDevice
				);
			}

			bool hasStencilComponent(VkFormat format) {
				return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
			}

			VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice) {
				VkPhysicalDeviceProperties physicalDeviceProperties;
				vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

				VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts,
					physicalDeviceProperties.limits.framebufferDepthSampleCounts);
				if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
				if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
				if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
				if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
				if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
				if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

				return VK_SAMPLE_COUNT_1_BIT;
			}

		}
	}
}
