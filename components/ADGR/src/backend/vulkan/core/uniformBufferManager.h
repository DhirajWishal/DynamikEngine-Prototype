#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_MANAGER_H
#ifdef DMK_USE_VULKAN

#include "vulkanFormat.h"
#include "backend/vulkan/core/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct DMKUniformBufferUpdateInfo {
				std::vector<VkDeviceMemory> bufferMemory = {};

				I32 currentImage = 0;

				std::vector<B1> move = { false, false };
				std::vector<B1> turn = { false, false };
				std::vector<B1> upDown = { false, false };
				std::vector<B1> rotation = { false, false };
				std::vector<F32> cPos = { 0.0f, 0.0f };

				F32 movementBias = 0.0075f;
				F32 upDownBias = 0.015f;
				F32 rotationBias = 2.5f;

				F32 trn = 0.0f;
				F32 mve = 0.0f;
				F32 up = 0.0f;
				F32 rot = 0.0f;

				B1 rotationLock = false;
				B1 verticalLock = false;
				B1 horizontalLock = false;
				B1 turnLock = false;
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

				void updateBuffer3D(ADGRVulkanDataContainer* container, std::deque<DMKEventContainer> eventContainers, vulkanFormat* gameObject, UI32 currentImage = 0);

				void clean(ADGRVulkanDataContainer* container, vulkanFormat* format);
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif	// !_DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H
