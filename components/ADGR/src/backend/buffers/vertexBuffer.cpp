/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "vertexBuffer.h"
#include "buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			VkVertexInputBindingDescription vertex::getBindingDescription() {
				VkVertexInputBindingDescription bindingDescription = {};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(vertex);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return bindingDescription;
			}

			std::array<VkVertexInputAttributeDescription, 2> vertex::getAttributeDescriptions() {
				std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(vertex, position);

				attributeDescriptions[1].binding = 0;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(vertex, color);

				return attributeDescriptions;
			}

			vertexBuffer::vertexBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
				VkBuffer* vertexBuffer, VkDeviceMemory* vertexBufferMemory) :
				myDevice(device), myPhysicalDevice(physicalDevice), myVertexBuffer(vertexBuffer),
				myVertexBufferMemory(vertexBufferMemory) {
			}

			void vertexBuffer::initBuffer(VkCommandPool commandPool, VkQueue graphicsQueue) {
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
