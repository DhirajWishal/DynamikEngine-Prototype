#pragma once

#include "core/Buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKColorImageCreateInfo {
				VkImage image;
				VkDeviceMemory imageMemory;
				VkImageView imageView;
				VkFormat colorFormat;
				VkSampleCountFlagBits msaaSamples;
			};

			class colorBufferManager : public Buffer {
			public:
				colorBufferManager() {}
				~colorBufferManager() {}

				void createBuffer(VkBuffer* buffer, size_t size,
					VkBufferUsageFlags flags, VkBufferUsageFlagBits memoryFlags);
				void initResources();
				void createResources(DMKColorImageCreateInfo info);

			private:
				VkDevice* m_device = &device;

				VkImage* m_colorImage = &colorImage;
				VkDeviceMemory* m_colorImageMemory = &colorImageMemory;
				VkImageView* m_colorImageView = &colorImageView;
			};
		}
	}
}
