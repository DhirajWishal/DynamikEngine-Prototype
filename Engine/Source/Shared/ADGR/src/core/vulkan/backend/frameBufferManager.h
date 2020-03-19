#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_FRAME_BUFFER_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_FRAME_BUFFER_MANAGER_H
#ifdef DMK_USE_VULKAN

#include "core/vulkan/backend/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct DMKFrameBuffersCreateInfo {
				ARRAY<VkFramebuffer>* frameBuffers;
				ARRAY<VkImageView> swapChainImageViews;
				VkImageView colorImageView;
				VkImageView depthImageView;
			};

			class frameBufferManager {
			public:
				frameBufferManager() {}
				~frameBufferManager() {}

				void init(ADGRVulkanDataContainer* container);
				void createFrameBuffers(ADGRVulkanDataContainer* container, DMKFrameBuffersCreateInfo info);

				void clear(ADGRVulkanDataContainer* container);

			private:
				I32 mySize = 0;
			};
		}
	}
}

#endif // DEBUG

#endif // !_DYNAMIK_ADGR_VULKAN_FRAME_BUFFER_H
