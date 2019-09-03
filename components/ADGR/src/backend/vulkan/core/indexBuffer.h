#pragma once

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

				void createIndexBuffer(DMKIndexBufferCreateInfo info);

				void deleteBuffer(DMKindexBufferDeleteInfo info);
			};
		}
	}
}
