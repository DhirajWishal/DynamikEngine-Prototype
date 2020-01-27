#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H
#ifdef DMK_USE_VULKAN

#include "backend/vulkan/core/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct DMKIndexBufferCreateInfo {
				std::vector<uint32_t> indices = {};

				VkBuffer* buffer = VK_NULL_HANDLE;
				VkDeviceMemory* buffereMemory = VK_NULL_HANDLE;
			};

			struct DMKindexBufferDeleteInfo {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			class indexBufferManager {
			public:
				indexBufferManager() {}
				~indexBufferManager() {}

				void createIndexBuffer(ADGRVulkanDataContainer* container, DMKIndexBufferCreateInfo info);

				void deleteBuffer(ADGRVulkanDataContainer* container, DMKindexBufferDeleteInfo info);
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H
