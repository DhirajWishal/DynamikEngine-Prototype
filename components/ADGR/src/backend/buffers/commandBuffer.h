#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "src/backend/interface.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class ADGR_API commandBuffer {
			public:
				commandBuffer(VkDevice* device, VkCommandPool* commandPool);
				~commandBuffer() {}

				void initBuffer(VkCommandPool commandPool, VkRenderPass renderPass,
					std::vector<VkFramebuffer> frameBuff, VkExtent2D swapChainExtent,
					VkPipeline graphicsPipeline, VkBuffer vertexBuffer, VkBuffer indexBuffer);

				void initCommandPool(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

				std::vector<VkCommandBuffer> getCommanBuffer() { return commandBuff; };

			private:
				VkDevice* myDevice;
				VkCommandPool* myCommandPool;

				uint32_t mySize = 30;
				std::vector<VkCommandBuffer> commandBuff;
			};
		}
	}
}