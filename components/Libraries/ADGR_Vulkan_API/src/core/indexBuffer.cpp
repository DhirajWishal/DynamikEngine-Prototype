#include "vkafx.h"
#include "indexBuffer.h"

#include "functions/bufferFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;
			
			void indexBufferManager::createIndexBuffer(DMKIndexBufferCreateInfo info) {
				VkDeviceSize bufferSize = sizeof(info.indices[0]) * info.indices.size();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				DMKCreateBufferInfo bufferInfo;
				bufferInfo.device = device;
				bufferInfo.physicalDevice = physicalDevice;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				functions::createBuffer(bufferInfo);

				void* data;
				vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, info.indices.data(), (size_t)bufferSize);
				vkUnmapMemory(device, stagingBufferMemory);

				DMKCreateBufferInfo idxBufferInfo;
				idxBufferInfo.device = device;
				idxBufferInfo.physicalDevice = physicalDevice;
				idxBufferInfo.bufferSize = bufferSize;
				idxBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
				idxBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				idxBufferInfo.buffer = info.buffer;
				idxBufferInfo.bufferMemory = info.buffereMemory;

				functions::createBuffer(idxBufferInfo);

				copyBuffer(device, stagingBuffer, *info.buffer, bufferSize, commandPool, graphicsQueue);

				vkDestroyBuffer(device, stagingBuffer, nullptr);
				vkFreeMemory(device, stagingBufferMemory, nullptr);
			}

			void indexBufferManager::deleteBuffer(DMKindexBufferDeleteInfo info) {
				vkDestroyBuffer(device, info.buffer, nullptr);
				vkFreeMemory(device, info.bufferMemory, nullptr);
			}
		}
	}
}
