#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H

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

				void createDescriptorSetLayout(ADGRVulkanDataContainer* container, DMKUniformBufferCreateDescriptorSetLayoutInfo info);
				void initDescriptorPool(ADGRVulkanDataContainer* container, VkDescriptorPool* descriptorPool);
				void initDescriptorSets(ADGRVulkanDataContainer* container, DMKDescriptorSetsInitInfo info);

				void createUniformBuffers(ADGRVulkanDataContainer* container, DMKUniformBufferCreateInfo info);

				void updateBuffer2D(ADGRVulkanDataContainer* container, DMKUniformBufferUpdateInfo info);
				void updateBuffer3D(ADGRVulkanDataContainer* container, DMKUniformBufferUpdateInfo info);

				void deleteBuffer(ADGRVulkanDataContainer* container, VkBuffer* buffer) override;

			private:
				const float movementBias = 0.01f;
				const float upDownBias = 0.01f;
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

#endif	// !_DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H
