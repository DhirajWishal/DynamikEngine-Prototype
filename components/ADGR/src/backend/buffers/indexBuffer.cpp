/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "indexBuffer.h"
#include "vertexBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			indexBuffer::indexBuffer(VkBuffer* indexBuffer, VkDeviceMemory* indexBufferMemory) :
				Buffer(indexBuffer, indexBufferMemory) {
			}

			void indexBuffer::initBuffer(VkQueue graphicsQueue) {
				VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;
				myVertexBuffer->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
					| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

				void* data;
				vkMapMemory(*myDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, indices.data(), (size_t)bufferSize);
				vkUnmapMemory(*myDevice, stagingBufferMemory);

				myVertexBuffer->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *myIndexBuffer, *myIndexBufferMemory);

				myVertexBuffer->copyBuffer(stagingBuffer, *myIndexBuffer, bufferSize, graphicsQueue);

				vkDestroyBuffer(*myDevice, stagingBuffer, nullptr);
				vkFreeMemory(*myDevice, stagingBufferMemory, nullptr);
			}

			void indexBuffer::deleteBuffer() {
				vkDestroyBuffer(*myDevice, *myIndexBuffer, nullptr);
				vkFreeMemory(*myDevice, *myIndexBufferMemory, nullptr);
			}

			void indexBuffer::setVertexBuffer(vertexBuffer* vertBuff) {
				myVertexBuffer = vertBuff;
			}
		}
	}
}
