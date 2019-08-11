#pragma once

#include "Renderer/backend/interface.h"
#include "core/utils/DMK_DataTypes.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class SMP_API depthBuffer {
			public:
				depthBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
					VkImage* depthImage, VkDeviceMemory* depthImgMem, VkImageView* depthImgView);
				~depthBuffer();

				void initBuffer(VkExtent2D swapChainExtent, VkCommandPool commandPool,
					VkQueue graphicsQueue, uint32 mipLevels, VkSampleCountFlagBits msaaSamples);
				void deleteBuffer();

				VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
					VkImageTiling tiling, VkFormatFeatureFlags features);
				VkFormat findDepthFormat();

			private:
				VkDevice* myDevice;
				VkPhysicalDevice* myPhysicalDevice;

				VkImage* myDepthImage;
				VkDeviceMemory* myDepthImageMemory;
				VkImageView* myDepthImageView;
			};

			VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice);
			bool hasStencilComponent(VkFormat format);
			VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
				VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice);
			VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
		}
	}
}
