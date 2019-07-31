#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "core/utils/DMK_DataTypes.h"
#include "core/core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class DMK_API Buffer {
			public:
				Buffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
					VkCommandPool* commandPool, std::vector<VkFramebuffer>& frameBuffer,
					VkBuffer* indexBuffer, VkDeviceMemory* indexBufferMemory,
					VkBuffer* vertexBuffer, VkDeviceMemory* vertexBufferMemory);
				Buffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
					VkCommandPool* commandPool);
				Buffer(std::vector<VkFramebuffer>& frameBuffer);
				Buffer(VkBuffer* indexBuffer, VkDeviceMemory* indexBufferMemory);
				Buffer(VkBuffer* vertexBuffer, VkDeviceMemory* vertexBufferMemory, uint32 type);
				Buffer() {}

				virtual ~Buffer() {};

				virtual void initBuffer() {}
				virtual void initBuffer(std::vector<VkImageView> swapChainImageViews,
					VkRenderPass renderPass, VkExtent2D swapChainExtent) {}
				//virtual void initBuffer(VkQueue graphicsQueue, vertexBuffer* myVertexBuffer) {}
				virtual void initBuffer(VkRenderPass renderPass, VkExtent2D swapChainExtent,
					VkPipeline graphicsPipeline) {}
				virtual void initBuffer(VkQueue graphicsQueue) {}

				//virtual void deleteBuffer() {}

				//virtual VkBuffer getBuffer() { return; }

			protected:
				VkDevice* myDevice = nullptr;
				VkPhysicalDevice* myPhysicalDevice = nullptr;

				VkCommandPool* myCommandPool = nullptr;

				std::vector<VkFramebuffer> myFrameBuffer = {};

				VkBuffer* myIndexBuffer = nullptr;
				VkDeviceMemory* myIndexBufferMemory = nullptr;

				VkBuffer* myVertexBuffer = nullptr;
				VkDeviceMemory* myVertexBufferMemory = nullptr;

			private:

			};
		}
	}
}