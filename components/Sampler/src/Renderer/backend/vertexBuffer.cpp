#include "smpafx.h"
#include "vertexBuffer.h"

#include "functions/bufferFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void vertexBufferManager::createVertexBuffer(DMKVertexBufferCreateInfo info) {
				VkDeviceSize bufferSize = sizeof(info.vertices[0]) * info.vertices.size();

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
				memcpy(data, info.vertices.data(), (size_t)bufferSize);
				vkUnmapMemory(device, stagingBufferMemory);

				DMKCreateBufferInfo vertBufferInfo;
				vertBufferInfo.device = device;
				vertBufferInfo.physicalDevice = physicalDevice;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = info.buffer;
				vertBufferInfo.bufferMemory = info.buffereMemory;

				functions::createBuffer(vertBufferInfo);

				copyBuffer(device, stagingBuffer, *info.buffer, bufferSize, commandPool, graphicsQueue);

				vkDestroyBuffer(device, stagingBuffer, nullptr);
				vkFreeMemory(device, stagingBufferMemory, nullptr);
			}

			void vertexBufferManager::deleteBuffer(DMKVertexBufferDeleteInfo info) {
				vkDestroyBuffer(device, info.buffer, nullptr);
				vkFreeMemory(device, info.bufferMemory, nullptr);
			}
		}
	}
}
