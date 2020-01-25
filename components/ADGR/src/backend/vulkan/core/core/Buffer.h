#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_BUFFER_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_BUFFER_H

#include "Core.h"

#include "backend/vulkan/core/functions/bufferFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class Buffer : public Core {
			public:
				Buffer() {}
				virtual ~Buffer() {}

				virtual void createBuffer(VkBuffer* buffer, size_t size,
					VkBufferUsageFlags flags, VkBufferUsageFlagBits memoryFlags) {}
				virtual void bindBuffer(VkBuffer* srcBuffer, VkBuffer* dstBuffer, size_t size, VkDeviceMemory bufferMemory,
					VkCommandPool myCommandPool) {}
				virtual void unbindBuffer(VkBuffer* buffer) {}
				virtual void deleteBuffer(ADGRVulkanDataContainer* container, VkBuffer* buffer) {}

			protected:
				virtual void createDescriptorSetLayout(VkDescriptorSetLayout* layout) {}
				virtual void initDescriptorPool() {}
			};

			/* STAGGING BUFFER */
			struct DMKStaggingBufferCreateInfo {
				VkDevice device = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

				VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				VkDeviceSize bufferSize = 0;

				VkMemoryPropertyFlags bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			};

			class staggingBuffer {
			public:
				staggingBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice, DMKStaggingBufferCreateInfo& info)
					: myDevice(device), myPhysicalDevice(physicalDevice), myInfo(info) {
					VkBufferCreateInfo bufferInfo = {};
					bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
					bufferInfo.size = myInfo.bufferSize;
					bufferInfo.usage = myInfo.usage;
					bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

					if (vkCreateBuffer(myDevice, &bufferInfo, nullptr, &myStaggingBuffer) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create buffer!");

					VkMemoryRequirements memRequirements;
					vkGetBufferMemoryRequirements(myDevice, myStaggingBuffer, &memRequirements);

					VkMemoryAllocateInfo allocInfo = {};
					allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
					allocInfo.allocationSize = memRequirements.size;
					allocInfo.memoryTypeIndex = functions::findMemoryType(memRequirements.memoryTypeBits, myInfo.bufferMemoryPropertyflags, myPhysicalDevice);

					if (vkAllocateMemory(myDevice, &allocInfo, nullptr, &myStaggingBufferMemory) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to allocate buffer memory!");

					vkBindBufferMemory(myDevice, myStaggingBuffer, myStaggingBufferMemory, 0);
				}

				~staggingBuffer() {
					vkDestroyBuffer(myDevice, myStaggingBuffer, nullptr);
					vkFreeMemory(myDevice, myStaggingBufferMemory, nullptr);
				}

				std::pair<VkBuffer, VkDeviceMemory> getBufferAndMemory() {
					return { myStaggingBuffer, myStaggingBufferMemory };
				}

				std::pair<VkBuffer*, VkDeviceMemory*> getBufferAddrAndMemoryAddr() {
					return { &myStaggingBuffer, &myStaggingBufferMemory };
				}

			private:
				VkDevice myDevice = VK_NULL_HANDLE;
				VkPhysicalDevice myPhysicalDevice = VK_NULL_HANDLE;

				DMKStaggingBufferCreateInfo myInfo = {};

				VkBuffer myStaggingBuffer = VK_NULL_HANDLE;
				VkDeviceMemory myStaggingBufferMemory = VK_NULL_HANDLE;
			};
		}
	}
}

#endif	// !_DYNAMIK_ADGR_VULKAN_RENDERER_BUFFER_H
