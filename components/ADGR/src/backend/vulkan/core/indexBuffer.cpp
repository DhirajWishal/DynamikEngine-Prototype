#include "adgrafx.h"
#include "indexBuffer.h"

#include "functions/bufferFunctions.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		namespace core {
			using namespace functions;

			void indexBufferManager::createIndexBuffer(ADGRVulkanDataContainer* container, vulkanFormat* format) {
				for (UI32 itr = 0; itr < format->myRendererFormat->myInternalFormat->myVertexBufferObjects.size(); itr++) {
					VkDeviceSize bufferSize = sizeof(format->myRendererFormat->myInternalFormat->myIndexBufferObjects[itr].at(0))
						* format->myRendererFormat->myInternalFormat->myIndexBufferObjects[itr].size();

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
					memcpy(data, format->myRendererFormat->myInternalFormat->myIndexBufferObjects[itr].data(), (size_t)bufferSize);
					vkUnmapMemory(container->device, stagingBufferMemory);

					VkBuffer _localIndexBuffer = VK_NULL_HANDLE;
					VkDeviceMemory _localIndexBufferMemory = VK_NULL_HANDLE;

					DMKCreateBufferInfo idxBufferInfo;
					idxBufferInfo.device = container->device;
					idxBufferInfo.physicalDevice = container->physicalDevice;
					idxBufferInfo.bufferSize = bufferSize;
					idxBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
					idxBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
					idxBufferInfo.buffer = &_localIndexBuffer;
					idxBufferInfo.bufferMemory = &_localIndexBufferMemory;

					functions::createBuffer(idxBufferInfo);

					copyBuffer(container->device, stagingBuffer, _localIndexBuffer, bufferSize, container->commandBufferContainer.commandPool, container->graphicsQueue);

					vkDestroyBuffer(container->device, stagingBuffer, nullptr);
					vkFreeMemory(container->device, stagingBufferMemory, nullptr);

					format->myIndexBuffers.push_back(_localIndexBuffer);
					format->myIndexBufferMemories.push_back(_localIndexBufferMemory);
				}
			}

			void indexBufferManager::clear(ADGRVulkanDataContainer* container, vulkanFormat* format) {
				for (UI32 itr = 0; itr < format->myIndexBuffers.size(); itr++) {
					vkDestroyBuffer(container->device, format->myIndexBuffers[itr], nullptr);
					vkFreeMemory(container->device, format->myIndexBufferMemories[itr], nullptr);
				}
			}
		}
	}
}
#endif // DMK_USE_VULKAN
