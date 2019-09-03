#pragma once

#include "core/Buffer.h"

#include "data structures/DMK_ADGR_DataStructures.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKBindCommandBufferInfo {
				std::vector<VkFramebuffer> frameBuffers = {};
				std::vector<std::vector<VkDescriptorSet>*> descriptorSets = {};

				VkBuffer indexBuffer = VK_NULL_HANDLE;

				std::vector<VkBuffer> vertexBuffers = {};
				std::vector<uint32_t> indices = {};

				DMKVulkanSkyboxCommandBufferBindData skybox;
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
