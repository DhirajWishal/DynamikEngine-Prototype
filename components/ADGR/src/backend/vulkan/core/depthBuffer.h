#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_DEPTH_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_DEPTH_BUFFER_H
#ifdef DMK_USE_VULKAN

#include "backend/vulkan/core/data structures/vulkan.h"

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

			class depthBufferManager {
			public:
				depthBufferManager() {}
				~depthBufferManager() {}

				void init(ADGRVulkanDataContainer* container);
				void createBuffer(ADGRVulkanDataContainer* container, DMKDepthBufferCreateInfo info);

				void clear(ADGRVulkanDataContainer* container);
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_DEPTH_BUFFER_H
