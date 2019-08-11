/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "smpafx.h"
#include "indexBuffer.h"
#include "buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			indexBuffer::indexBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkBuffer* buffer,
				VkDeviceMemory* bufferMemory) :
				myDevice(device), myPhysicalDevice(physicalDevice), myIndexBuffer(buffer), myIndexBufferMemory(bufferMemory) {
			}

			void indexBuffer::initBuffer(vertexBuffer* vertexBuffer, model myModel, VkCommandPool commandPool,
				VkQueue graphicsQueue) {
				std::vector<uint32> indices = myModel.getIndeices();
				VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;
				createBuffer(*myDevice, *myPhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
					| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

				void* data;
				vkMapMemory(*myDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, indices.data(), (size_t)bufferSize);
				vkUnmapMemory(*myDevice, stagingBufferMemory);

				createBuffer(*myDevice, *myPhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *myIndexBuffer, *myIndexBufferMemory);

				copyBuffer(*myDevice, stagingBuffer, *myIndexBuffer, bufferSize, commandPool, graphicsQueue);

				vkDestroyBuffer(*myDevice, stagingBuffer, nullptr);
				vkFreeMemory(*myDevice, stagingBufferMemory, nullptr);
			}

			void indexBuffer::deleteIndexBuffer() {
				vkDestroyBuffer(*myDevice, *myIndexBuffer, nullptr);
				vkFreeMemory(*myDevice, *myIndexBufferMemory, nullptr);
			}
		}
	}
}
