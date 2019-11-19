#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_COLOR_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_COLOR_BUFFER_H

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
				void initResources(ADGRVulkanDataContainer* container);
				void createResources(ADGRVulkanDataContainer* container, DMKColorImageCreateInfo info);

				void clear(ADGRVulkanDataContainer* container) override;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_COLOR_BUFFER_H
