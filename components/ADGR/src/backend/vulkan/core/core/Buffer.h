#pragma once

#include "Core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class Buffer : public Core {
			public:
				Buffer() {}
				virtual ~Buffer() {}

				virtual void createBuffer(VkBuffer* buffer, size_t size,
					VkBufferUsageFlags flags, VkBufferUsageFlagBits memoryFlags) {}
				virtual void bindBuffer(VkBuffer* srcBuffer, VkBuffer* dstBuffer, size_t size, VkDeviceMemory bufferMemory,
					VkCommandPool myCommandPool) {}
				virtual void unbindBuffer(VkBuffer* buffer) {}
				virtual void deleteBuffer(ADGRVulkanDataContainer* container, VkBuffer* buffer) {}

			protected:
				virtual void createDescriptorSetLayout(VkDescriptorSetLayout* layout) {}
				virtual void initDescriptorPool() {}
			};
		}
	}
}