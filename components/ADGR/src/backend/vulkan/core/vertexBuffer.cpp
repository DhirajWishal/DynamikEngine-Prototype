#include "adgrafx.h"
#include "vertexBuffer.h"

#include "functions/bufferFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void vertexBufferManager::createVertexBuffer(ADGRVulkanDataContainer* container, DMKVertexBufferCreateInfo info) {
				VkDeviceSize bufferSize = sizeof(info.vertices[0]) * info.vertices.size();

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

				void* data;
				vkMapMemory(container->device, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, info.vertices.data(), (size_t)bufferSize);
				vkUnmapMemory(container->device, stagingBufferMemory);

				DMKCreateBufferInfo vertBufferInfo;
				vertBufferInfo.device = container->device;
				vertBufferInfo.physicalDevice = container->physicalDevice;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = info.buffer;
				vertBufferInfo.bufferMemory = info.bufferMemory;

				functions::createBuffer(vertBufferInfo);

				copyBuffer(container->device, stagingBuffer, *info.buffer, bufferSize, container->commandBufferContainer.commandPool, container->graphicsQueue);

				vkDestroyBuffer(container->device, stagingBuffer, nullptr);
				vkFreeMemory(container->device, stagingBufferMemory, nullptr);
			}

			void vertexBufferManager::deleteBuffer(ADGRVulkanDataContainer* container, DMKVertexBufferDeleteInfo info) {
				vkDestroyBuffer(container->device, info.buffer, nullptr);
				vkFreeMemory(container->device, info.bufferMemory, nullptr);
			}
		}
	}
}