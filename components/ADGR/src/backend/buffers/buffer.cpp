/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			Buffer::Buffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
				VkCommandPool* commandPool, std::vector<VkFramebuffer>& frameBuffer,
				VkBuffer* indexBuffer, VkDeviceMemory* indexBufferMemory,
				VkBuffer* vertexBuffer, VkDeviceMemory* vertexBufferMemory) :
				myDevice(device), myPhysicalDevice(physicalDevice), myCommandPool(commandPool),
				myFrameBuffer(frameBuffer), myIndexBuffer(indexBuffer), myIndexBufferMemory(indexBufferMemory),
				myVertexBuffer(vertexBuffer), myVertexBufferMemory(vertexBufferMemory) {
			}

			Buffer::Buffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
				VkCommandPool* commandPool) :
				myDevice(device), myPhysicalDevice(physicalDevice), myCommandPool(commandPool) {
			}

			Buffer::Buffer(std::vector<VkFramebuffer>& frameBuffer) : myFrameBuffer(frameBuffer) {
			}

			Buffer::Buffer(VkBuffer* indexBuffer, VkDeviceMemory* indexBufferMemory) :
				myIndexBuffer(indexBuffer), myIndexBufferMemory(indexBufferMemory) {
			}

			Buffer::Buffer(VkBuffer* vertexBuffer, VkDeviceMemory* vertexBufferMemory, uint32 type) :
				myVertexBuffer(vertexBuffer), myVertexBufferMemory(vertexBufferMemory) {
			}
		}
	}
}
