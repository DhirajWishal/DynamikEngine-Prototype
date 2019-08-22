#pragma once

#include "core/Buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKIndexBufferCreateInfo {
				std::vector<uint32_t> indices;

				VkBuffer* buffer;
				VkDeviceMemory* buffereMemory;
			};

			class indexBufferManager : public Buffer {
			public:
				indexBufferManager() {}
				~indexBufferManager() {}

				void createIndexBuffer(DMKIndexBufferCreateInfo info);
			};
		}
	}
}
