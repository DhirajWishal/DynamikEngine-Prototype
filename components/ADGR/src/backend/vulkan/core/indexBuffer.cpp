#include "adgrafx.h"
#include "indexBuffer.h"

#include "functions/bufferFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void indexBufferManager::createIndexBuffer(ADGRVulkanDataContainer* container, DMKIndexBufferCreateInfo info) {
				VkDeviceSize bufferSize = sizeof(info.indices[0]) * info.indices.size();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				DMKCreateBufferInfo bufferInfo;
				bufferInfo.device = container->device;
				bufferInfo.physicalDevice = container->physicalDevice;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				functions::createBuffer(bufferInfo);

				void* data = nullptr;
				vkMapMemory(container->device, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, info.indices.data(), (size_t)bufferSize);
				vkUnmapMemory(container->device, stagingBufferMemory);

				DMKCreateBufferInfo idxBufferInfo;
				idxBufferInfo.device = container->device;
				idxBufferInfo.physicalDevice = container->physicalDevice;
				idxBufferInfo.bufferSize = bufferSize;
				idxBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
				idxBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				idxBufferInfo.buffer = info.buffer;
				idxBufferInfo.bufferMemory = info.buffereMemory;

				functions::createBuffer(idxBufferInfo);

				copyBuffer(container->device, stagingBuffer, *info.buffer, bufferSize, container->commandBufferContainer.commandPool, container->graphicsQueue);

				vkDestroyBuffer(container->device, stagingBuffer, nullptr);
				vkFreeMemory(container->device, stagingBufferMemory, nullptr);
			}

			void indexBufferManager::deleteBuffer(ADGRVulkanDataContainer* container, DMKindexBufferDeleteInfo info) {
				vkDestroyBuffer(container->device, info.buffer, nullptr);
				vkFreeMemory(container->device, info.bufferMemory, nullptr);
			}
		}
	}
}