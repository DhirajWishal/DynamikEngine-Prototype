#pragma once

#include "core/Buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKBindCommandBufferInfo {
				std::vector<VkFramebuffer> frameBuffers;
				std::vector<std::vector<VkDescriptorSet>*> descriptorSets;

				VkBuffer indexBuffer;

				std::vector<VkBuffer> vertexBuffers;
				std::vector<uint32_t> indices;
			};

			class commandBufferManager : public Buffer {
			public:
				commandBufferManager() {}
				~commandBufferManager() {}

				void createBuffer(VkBuffer* buffer, size_t size,
					VkBufferUsageFlags flags, VkBufferUsageFlagBits memoryFlags) override;
				void initCommandPool();

				void bindCommands(DMKBindCommandBufferInfo info);

				VkCommandBuffer* getBuffer(int index) { return &commandBuffers[index]; }
				VkCommandPool getCommandPool() { return *m_commandPool; }

			private:
				VkDevice* m_device = &device;
				VkCommandPool* m_commandPool = &commandPool;
			};
		}
	}
}
