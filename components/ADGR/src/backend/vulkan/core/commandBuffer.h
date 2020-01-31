#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H
#ifdef DMK_USE_VULKAN

#include "data structures/DMK_ADGR_DataStructures.h"
#include "vulkanFormat.h"
#include "core/data store/containers.h"
#include "core/data store/objectSpecifications.h"
#include "backend/vulkan/core/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class commandBufferManager {
			public:
				commandBufferManager() {}
				~commandBufferManager() {}

				void initCommandPool(ADGRVulkanDataContainer* container);
				void bindCommands(ADGRVulkanDataContainer* container, std::vector<vulkanFormat>* formats);
				void clear(ADGRVulkanDataContainer* container);

			private:
				void drawVertex(VkCommandBuffer buffer, int index, vulkanFormat* format, VkDeviceSize* offsets);
				void drawIndexed(VkCommandBuffer buffer, int index, vulkanFormat* format, VkDeviceSize* offsets);
			};

			/* ONE TIME COMMAND BUFFER CLASS */
			class oneTimeCommandBufferManager {
			public:
				oneTimeCommandBufferManager(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, uint32_t count = 1);
				~oneTimeCommandBufferManager();

				void destroyBuffers();
				std::vector<VkCommandBuffer> getCommandBuffers();

			private:
				uint32_t myCount = 0;

				VkDevice myDevice = VK_NULL_HANDLE;
				VkCommandPool myCommandPool = VK_NULL_HANDLE;
				VkQueue myGraphicsQueue = VK_NULL_HANDLE;

				std::vector<VkCommandBuffer> myCommandBuffers = {};

				bool isDestroyed = false;
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif	// !_DYNAMIK_ADGR_VULKAN_COMMAND_BUFFER_H
