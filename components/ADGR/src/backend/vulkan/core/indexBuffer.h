#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H

#include "core/Buffer.h"

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

			class indexBufferManager : public Buffer {
			public:
				indexBufferManager() {}
				~indexBufferManager() {}

				void createIndexBuffer(ADGRVulkanDataContainer* container, DMKIndexBufferCreateInfo info);

				void deleteBuffer(ADGRVulkanDataContainer* container, DMKindexBufferDeleteInfo info);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_INDEX_BUFFER_H
