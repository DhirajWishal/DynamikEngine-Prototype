#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_FRAME_BUFFER_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_FRAME_BUFFER_MANAGER_H

#include "Objects/VulkanCoreObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanFrameBufferInitInfo {
				ARRAY<VkImageView> preAttachments;
				ARRAY<VkImageView> additionalAttachments;
			};

			class VulkanFrameBufferManager {
			public:
				VulkanFrameBufferManager() {}
				~VulkanFrameBufferManager() {}

				void initialize(POINTER<VulkanCoreObject> core, ADGRVulkanFrameBufferInitInfo info);
				void terminate(POINTER<VulkanCoreObject> core);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_FRAME_BUFFER_H
