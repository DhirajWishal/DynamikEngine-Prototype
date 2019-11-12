#pragma once

#include "core/Buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct DMKDepthBufferCreateInfo {
				VkImage* image;
				VkDeviceMemory* imageMemory;
				VkImageView imageViews;
				VkFormat imageFormat;
				VkSampleCountFlagBits msaaSamples;
			};

			class depthBufferManager : public Buffer {
			public:
				depthBufferManager() {}
				~depthBufferManager() {}

				void init(ADGRVulkanDataContainer* container) override;
				void createBuffer(ADGRVulkanDataContainer* container, DMKDepthBufferCreateInfo info);

				void clear(ADGRVulkanDataContainer* container) override;
			};
		}
	}
}
