/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "smpafx.h"
#include "vertexBuffer.h"
#include "buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			vertexBuffer::vertexBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
				VkBuffer* vertexBuffer, VkDeviceMemory* vertexBufferMemory) :
				myDevice(device), myPhysicalDevice(physicalDevice), myVertexBuffer(vertexBuffer),
				myVertexBufferMemory(vertexBufferMemory) {
			}

			void vertexBuffer::initBuffer(VkCommandPool commandPool, VkQueue graphicsQueue, model myModel) {
				std::vector<vertex> vertices = myModel.getVertices();
				VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;
				createBuffer(*myDevice, *myPhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
					| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

				void* data;
				vkMapMemory(*myDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, vertices.data(), (size_t)bufferSize);
				vkUnmapMemory(*myDevice, stagingBufferMemory);

				createBuffer(*myDevice, *myPhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *myVertexBuffer, *myVertexBufferMemory);

				copyBuffer(*myDevice, stagingBuffer, *myVertexBuffer, bufferSize, commandPool, graphicsQueue);

				vkDestroyBuffer(*myDevice, stagingBuffer, nullptr);
				vkFreeMemory(*myDevice, stagingBufferMemory, nullptr);
			}

			void vertexBuffer::deleteVertexBuffer() {
				vkDestroyBuffer(*myDevice, *myVertexBuffer, nullptr);
				vkFreeMemory(*myDevice, *myVertexBufferMemory, nullptr);
			}
		}
	}
}
