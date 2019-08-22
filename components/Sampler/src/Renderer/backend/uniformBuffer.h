#pragma once

#include "core/Buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKUniformBufferCreateInfo {
				std::vector<VkBuffer>* buffers;
				std::vector<VkDeviceMemory>* bufferMemories;
			};

			struct DMKDescriptorSetsInitInfo {
				std::vector<VkBuffer>* uniformBuffers;
			};

			class uniformBufferManager : public Buffer {
			public:
				uniformBufferManager() {}
				~uniformBufferManager() {}

				void createDescriptorSetLayout() override;
				void initDescriptorPool() override;
				void initDescriptorSets(DMKDescriptorSetsInitInfo info);

				void createUniformBuffers(DMKUniformBufferCreateInfo info);

				void deleteBuffer(VkBuffer* buffer) override;

			private:
				VkDevice* m_device = &device;
				VkDescriptorSetLayout* m_descriptorSetLayout = &descriptorSetLayout;
				VkDescriptorPool* m_descriptorPool = &descriptorPool;
				VkCommandPool* m_commandPool = &commandPool;

				std::vector<VkDescriptorSet>* m_descriptorSets = &descriptorSets;
			};
		}
	}
}
