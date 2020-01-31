#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H
#ifdef DMK_USE_VULKAN

#include "vulkanFormat.h"
#include "backend/vulkan/core/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
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

			class uniformBufferManager {
			public:
				uniformBufferManager() {}
				~uniformBufferManager() {}

				void createDescriptorSetLayout(ADGRVulkanDataContainer* container, vulkanFormat* format);
				void createDescriptorSetLayout(ADGRVulkanDataContainer* container, VkDescriptorSetLayout* descriptorSetLayout);
				void initDescriptorPool(ADGRVulkanDataContainer* container, vulkanFormat* format);
				void initDescriptorSets(ADGRVulkanDataContainer* container, vulkanFormat* format);

				void createUniformBuffers(ADGRVulkanDataContainer* container, vulkanFormat* format);
				void createUniformBuffers(ADGRVulkanDataContainer* container,
					std::vector<VkBuffer>* uniformBuffers, std::vector<VkDeviceMemory>* uniformBufferMemories);

				void updateBuffer3D(ADGRVulkanDataContainer* container, std::deque<DMKEventContainer> eventContainers, vulkanFormat* gameObject, uint32_t currentImage = 0);

				void clean(ADGRVulkanDataContainer* container, vulkanFormat* format);
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif	// !_DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H
