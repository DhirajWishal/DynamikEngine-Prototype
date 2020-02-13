#include "adgrafx.h"
#include "vertexBufferManager.h"

#include "functions/bufferFunctions.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void vertexBufferManager::createVertexBuffer(ADGRVulkanDataContainer* container, vulkanFormat* format) {
				for (UI32 itr = 0; itr < format->myRendererFormat->myInternalFormat->myVertexBufferObjects.size(); itr++) {
					VkDeviceSize bufferSize = sizeof(format->myRendererFormat->myInternalFormat->myVertexBufferObjects[itr].at(0))
						* format->myRendererFormat->myInternalFormat->myVertexBufferObjects[itr].size();

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
					memcpy(data, format->myRendererFormat->myInternalFormat->myVertexBufferObjects[itr].data(), (size_t)bufferSize);
					vkUnmapMemory(container->device, stagingBufferMemory);

					VkBuffer _localVertexBuffer = VK_NULL_HANDLE;
					VkDeviceMemory _localVertexBufferMemory = VK_NULL_HANDLE;

					DMKCreateBufferInfo vertBufferInfo;
					vertBufferInfo.device = container->device;
					vertBufferInfo.physicalDevice = container->physicalDevice;
					vertBufferInfo.bufferSize = bufferSize;
					vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
					vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
					vertBufferInfo.buffer = &_localVertexBuffer;
					vertBufferInfo.bufferMemory = &_localVertexBufferMemory;

					functions::createBuffer(vertBufferInfo);

					copyBuffer(container->device, stagingBuffer, _localVertexBuffer, bufferSize, container->commandBufferContainer.commandPool, container->graphicsQueue);

					vkDestroyBuffer(container->device, stagingBuffer, nullptr);
					vkFreeMemory(container->device, stagingBufferMemory, nullptr);

					format->myVertexBuffers.push_back(_localVertexBuffer);
					format->myVertexBufferMemories.push_back(_localVertexBufferMemory);
				}
			}

			void vertexBufferManager::clear(ADGRVulkanDataContainer* container, vulkanFormat* format) {
				for (UI32 itr = 0; itr < format->myVertexBuffers.size(); itr++) {
					vkDestroyBuffer(container->device, format->myVertexBuffers[itr], nullptr);
					vkFreeMemory(container->device, format->myVertexBufferMemories[itr], nullptr);
				}
			}
		}
	}
}
#endif // DMK_USE_VULKAN
