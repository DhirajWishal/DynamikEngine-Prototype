#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_MANAGER_H

#include "core/vulkan/backend/data structures/vulkan.h"
#include "vulkanFormat.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		namespace core {
			class indexBufferManager {
			public:
				indexBufferManager() {}
				~indexBufferManager() {}

				void createIndexBuffer(ADGRVulkanDataContainer* container, vulkanFormat* format);
				void clear(ADGRVulkanDataContainer* container, vulkanFormat* format);
			};
		}
	}
}
#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H