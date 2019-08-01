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

			class ADGR_API frameBuffer {
			public:
				frameBuffer(VkDevice* device);
				~frameBuffer() {}

				void initBuffer(std::vector<VkImageView> swapChainImageViews,
					VkRenderPass renderPass, VkExtent2D swapChainExtent);

				void deleteFrameBuffers();

				uint32_t getFrameBuffersSize() const { return frameBuff.size(); }
				VkFramebuffer getFrame(uint32_t pos) const { return frameBuff[pos]; }
				std::vector<VkFramebuffer> getFrameBuffers() const { return frameBuff; }

			private:
				VkDevice* myDevice;

				std::vector<VkFramebuffer> frameBuff;

				uint32_t mySize = 30;
			};
		}
	}
}
