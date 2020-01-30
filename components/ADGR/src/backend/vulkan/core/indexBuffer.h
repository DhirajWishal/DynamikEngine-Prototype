#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H

#include "backend/vulkan/core/data structures/vulkan.h"
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
				void deleteBuffer(ADGRVulkanDataContainer* container, vulkanFormat* format);
			};
		}
	}
}
#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H
