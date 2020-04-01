#include "adgrafx.h"
#include "VulkanVertexBufferManager.h"

#include "VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanVertexBufferManager::initialize(POINTER<VulkanCoreObject> core, ARRAY<Vertex>* vertexes, POINTER<VulkanRenderObject> object)
			{
				ADGRVulkanVertexBufferContainer _container;
				_container.count = vertexes->size();
				VkDeviceSize bufferSize = _container.count * sizeof(Vertex);

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;

				ADGRCreateBufferInfo bufferInfo;
				bufferInfo.bufferSize = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				bufferInfo.buffer = &stagingBuffer;
				bufferInfo.bufferMemory = &stagingBufferMemory;

				VulkanFunctions::createBuffer(core, bufferInfo);

				void* data = nullptr;
				vkMapMemory(core->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, vertexes->data(), (size_t)bufferSize);
				vkUnmapMemory(core->logicalDevice, stagingBufferMemory);

				ADGRCreateBufferInfo vertBufferInfo;
				vertBufferInfo.bufferSize = bufferSize;
				vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
				vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				vertBufferInfo.buffer = &_container.buffer;
				vertBufferInfo.bufferMemory = &_container.bufferMemory;

				VulkanFunctions::createBuffer(core, vertBufferInfo);

				VulkanFunctions::copyBuffer(core, stagingBuffer, _container.buffer, bufferSize);

				vkDestroyBuffer(core->logicalDevice, stagingBuffer, nullptr);
				vkFreeMemory(core->logicalDevice, stagingBufferMemory, nullptr);

				object->vertexBuffers.pushBack(_container);
			}

			void VulkanVertexBufferManager::terminate(POINTER<VulkanCoreObject> core, ADGRVulkanVertexBufferContainer container)
			{
				vkDestroyBuffer(core->logicalDevice, container.buffer, nullptr);
				vkFreeMemory(core->logicalDevice, container.bufferMemory, nullptr);
			}
		}
	}
}