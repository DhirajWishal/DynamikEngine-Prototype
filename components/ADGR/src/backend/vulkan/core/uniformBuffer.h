#pragma once

#include "core/Buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKUniformBufferCreateInfo {
				std::vector<VkBuffer>* buffers = {};
				std::vector<VkDeviceMemory>* bufferMemories = {};
			};

			struct DMKDescriptorSetsInitInfo {
				std::vector<VkBuffer>* uniformBuffers = {};
				std::vector<VkDescriptorSet>* descriptorSets = {};
				VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

				VkImageView textureImageView = VK_NULL_HANDLE;
				VkSampler textureSampler = VK_NULL_HANDLE;
				VkDescriptorSetLayout* layout = VK_NULL_HANDLE;

				std::vector<int> bindIndex = { 0, 1 };
			};

			struct DMKUniformBufferUpdateInfo {
				std::vector<VkDeviceMemory> bufferMemory = {};

				int currentImage = 0;

				std::vector<bool> move = { false, false };
				std::vector<bool> turn = { false, false };
				std::vector<bool> upDown = { false, false };
				std::vector<bool> rotation = { false, false };
				std::vector<float> cPos = { 0.0f, 0.0f };
			};

			struct DMKUniformBufferCreateDescriptorSetLayoutInfo {
				VkDescriptorSetLayout* layout = {};
				std::vector<int> bindIndex = { 0, 1, 2 };
			};

			class uniformBufferManager : public Buffer {
			public:
				uniformBufferManager() {}
				~uniformBufferManager() {}

				void createDescriptorSetLayout(DMKUniformBufferCreateDescriptorSetLayoutInfo info);
				void initDescriptorPool(VkDescriptorPool* descriptorPool);
				void initDescriptorSets(DMKDescriptorSetsInitInfo info);

				void createUniformBuffers(DMKUniformBufferCreateInfo info);

				void updateBuffer2D(DMKUniformBufferUpdateInfo info);
				void updateBuffer3D(DMKUniformBufferUpdateInfo info);

				void deleteBuffer(VkBuffer* buffer) override;

			private:
				VkDevice* m_device = &device;
				VkDescriptorSetLayout* m_descriptorSetLayout = &descriptorSetLayout;
				VkDescriptorPool* m_descriptorPool = &descriptorPool;
				VkCommandPool* m_commandPool = &commandPool;

				std::vector<VkDescriptorSet>* m_descriptorSets = &descriptorSets;


				const float movementBias = 0.005f;
				const float upDownBias = 0.005f;
				const float rotationBias = 2.5f;
				//const float rotationBias	= 0.05f;

				float trn = 0.0f;
				float mve = 0.0f;
				float up = 0.0f;
				float rotation = 0.0f;
				//float rotation			= 0.0f;
			};
		}
	}
}
