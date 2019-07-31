#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class DMK_API commandBuffer : public Buffer {
			public:
				commandBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
					VkCommandPool* commandPool);
				~commandBuffer() {}

				void initBuffer(VkRenderPass renderPass, VkExtent2D swapChainExtent,
					VkPipeline graphicsPipeline) override;
				void initCommandPool(VkSurfaceKHR surface);

				inline std::vector<VkCommandBuffer> getcommandBuffers() { return myCommandBuffer; };

			private:
				size_t mySize = 30;
				std::vector<VkCommandBuffer> myCommandBuffer;
			};
		}
	}
}