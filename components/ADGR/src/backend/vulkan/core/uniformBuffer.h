#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H

#include "vulkanFormat.h"
#include "backend/vulkan/core/data structures/vulkan.h"

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

				float movementBias = 0.0075f;
				float upDownBias = 0.015f;
				float rotationBias = 2.5f;

				float trn = 0.0f;
				float mve = 0.0f;
				float up = 0.0f;
				float rot = 0.0f;

				bool rotationLock = false;
				bool verticalLock = false;
				bool horizontalLock = false;
				bool turnLock = false;
			};

			struct DMKUniformBufferCreateDescriptorSetLayoutInfo {
				VkDescriptorSetLayout* layout = {};
				std::vector<int> bindIndex = { 0, 1, 2 };
			};

			class uniformBufferManager {
			public:
				uniformBufferManager() {}
				~uniformBufferManager() {}

				void createDescriptorSetLayout(ADGRVulkanDataContainer* container, DMKUniformBufferCreateDescriptorSetLayoutInfo info);
				void initDescriptorPool(ADGRVulkanDataContainer* container, VkDescriptorPool* descriptorPool);
				void initDescriptorSets(ADGRVulkanDataContainer* container, DMKDescriptorSetsInitInfo info);

				void createUniformBuffers(ADGRVulkanDataContainer* container, DMKUniformBufferCreateInfo info);

				void updateBuffer3D(ADGRVulkanDataContainer* container, std::deque<DMKEventContainer>& eventContainers, vulkanFormat* gameObject, uint32_t currentImage = 0);

				void deleteBuffer(ADGRVulkanDataContainer* container, VkBuffer* buffer);
			};
		}
	}
}

#endif	// !_DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H
