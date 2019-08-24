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
				std::vector<VkDescriptorSet>* descriptorSets;

				VkImageView textureImageView;
				VkSampler textureSampler;
			};

			struct DMKUniformBufferUpdateInfo {
				std::vector<VkDeviceMemory> bufferMemory;

				int currentImage;

				std::vector<bool> move = {false, false};
				std::vector<bool> turn = {false, false};
			};

			class uniformBufferManager : public Buffer {
			public:
				uniformBufferManager() {}
				~uniformBufferManager() {}

				void createDescriptorSetLayout() override;
				void initDescriptorPool() override;
				void initDescriptorSets(DMKDescriptorSetsInitInfo info);

				void createUniformBuffers(DMKUniformBufferCreateInfo info);

				void updateBuffer(DMKUniformBufferUpdateInfo info);

				void deleteBuffer(VkBuffer* buffer) override;

			private:
				VkDevice* m_device = &device;
				VkDescriptorSetLayout* m_descriptorSetLayout = &descriptorSetLayout;
				VkDescriptorPool* m_descriptorPool = &descriptorPool;
				VkCommandPool* m_commandPool = &commandPool;

				std::vector<VkDescriptorSet>* m_descriptorSets = &descriptorSets;


				const float movementBias = 0.05f;

				float trn = 0.0f;
				float mve = 0.0f;
			};
		}
	}
}
