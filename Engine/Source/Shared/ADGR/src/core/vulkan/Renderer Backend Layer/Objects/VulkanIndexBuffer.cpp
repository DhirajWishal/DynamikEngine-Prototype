#include "adgrafx.h"
#include "VulkanIndexBuffer.h"

#include "VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanIndexBuffer::initialize(VulkanDevice device, VulkanCommandBuffer commandBuffer, VulkanQueue queue, ARRAY<UI32>* indexes)
			{
				count = indexes->size();
				VkDeviceSize bufferSize = count * indexes->typeSize();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRCreateBufferInfo bufferInfo;
				bufferInfo.device = device;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanFunctions::createBuffer(bufferInfo);

				void* data = nullptr;
				vkMapMemory(device.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, indexes->data(), (size_t)bufferSize);
				vkUnmapMemory(device.logicalDevice, stagingBufferMemory);

				ADGRCreateBufferInfo vertBufferInfo;
				vertBufferInfo.device = device;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &buffer;
				vertBufferInfo.bufferMemory = &bufferMemory;

				VulkanFunctions::createBuffer(vertBufferInfo);

				VulkanFunctions::copyBuffer(device.logicalDevice, stagingBuffer, buffer, bufferSize, commandBuffer.commandPool, queue.graphicsQueue);

				vkDestroyBuffer(device.logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(device.logicalDevice, stagingBufferMemory, nullptr);
			}

			void VulkanIndexBuffer::terminate(VulkanDevice device)
			{
				vkDestroyBuffer(device.logicalDevice, buffer, nullptr);
				vkFreeMemory(device.logicalDevice, bufferMemory, nullptr);
			}
		}
	}
}