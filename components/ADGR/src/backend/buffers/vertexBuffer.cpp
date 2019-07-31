/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "vertexBuffer.h"

#define VERTEX_BUFFER 1

namespace Dynamik {
	namespace ADGR {
		namespace core {

			vertexBuffer::vertexBuffer(VkBuffer* vertexBuffer, VkDeviceMemory* vertexBufferMemory, uint32 type) :
				Buffer(vertexBuffer, vertexBufferMemory, type) {
			}

			void vertexBuffer::initBuffer(VkQueue graphicsQueue) {
				VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

				VkBuffer stagingBuffer;
				VkDeviceMemory stagingBufferMemory;
				createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
					| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

				void* data;
				vkMapMemory(*myDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
				memcpy(data, vertices.data(), (size_t)bufferSize);
				vkUnmapMemory(*myDevice, stagingBufferMemory);

				createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *myVertexBuffer, *myVertexBufferMemory);

				copyBuffer(stagingBuffer, *myVertexBuffer, bufferSize, graphicsQueue);

				vkDestroyBuffer(*myDevice, stagingBuffer, nullptr);
				vkFreeMemory(*myDevice, stagingBufferMemory, nullptr);
			}

			void vertexBuffer::deleteBuffer() {
				vkDestroyBuffer(*myDevice, *myVertexBuffer, nullptr);
				vkFreeMemory(*myDevice, *myVertexBufferMemory, nullptr);
			}

			uint32 vertexBuffer::findMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties) {
				VkPhysicalDeviceMemoryProperties memProperties;
				vkGetPhysicalDeviceMemoryProperties(*myPhysicalDevice, &memProperties);

				for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
					if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags
						& properties) == properties)
						return i;

				throw std::runtime_error("failed to find suitable memory type!");
			}

			void vertexBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
				VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
				VkBufferCreateInfo bufferInfo = {};
				bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferInfo.size = size;
				bufferInfo.usage = usage;
				bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				if (vkCreateBuffer(*myDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
					throw std::runtime_error("failed to create buffer!");

				VkMemoryRequirements memRequirements;
				vkGetBufferMemoryRequirements(*myDevice, buffer, &memRequirements);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

				if (vkAllocateMemory(*myDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
					throw std::runtime_error("failed to allocate buffer memory!");

				vkBindBufferMemory(*myDevice, buffer, bufferMemory, 0);


			}

			void vertexBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
				VkQueue graphicsQueue) {
				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandPool = *myCommandPool;
				allocInfo.commandBufferCount = 1;

				VkCommandBuffer commandBuffer;
				vkAllocateCommandBuffers(*myDevice, &allocInfo, &commandBuffer);

				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

				vkBeginCommandBuffer(commandBuffer, &beginInfo);

				VkBufferCopy copyRegion = {};
				copyRegion.srcOffset = 0; // Optional
				copyRegion.dstOffset = 0; // Optional
				copyRegion.size = size;
				vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

				vkEndCommandBuffer(commandBuffer);

				VkSubmitInfo submitInfo = {};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = 1;
				submitInfo.pCommandBuffers = &commandBuffer;

				vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
				vkQueueWaitIdle(graphicsQueue);

				vkFreeCommandBuffers(*myDevice, *myCommandPool, 1, &commandBuffer);
			}

			void vertexBuffer::copyData() {

			}

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
		}
	}
}
