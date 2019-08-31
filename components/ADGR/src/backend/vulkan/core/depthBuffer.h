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

			class ADGR_API depthBufferManager : public Buffer {
			public:
				depthBufferManager() {}
				~depthBufferManager() {}

				void init() override;
				void createBuffer(DMKDepthBufferCreateInfo info);

				void clear() override;

			private:
				VkDevice* m_device = &device;

				VkImage *m_depthImage = &depthImage;
				VkDeviceMemory *m_depthImageMemory = &depthImageMemory;
				VkImageView *m_depthImageView = &depthImageView;
			};
		}
	}
}
