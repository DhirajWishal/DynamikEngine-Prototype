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

			class DMK_API frameBuffer : public Buffer {
			public:
				frameBuffer();
				~frameBuffer() {}

				void initBuffer(std::vector<VkImageView> swapChainImageViews,
					VkRenderPass renderPass, VkExtent2D swapChainExtent) override;

				void deleteBuffers();

				inline size_t getFrameBuffersSize() const { return myFrameBuffer.size(); }
				inline VkFramebuffer getFrame(uint32_t pos) const { return myFrameBuffer[pos]; }
				inline std::vector<VkFramebuffer> getFrameBuffers() const { return myFrameBuffer; }

			private:
				size_t mySize = 30;
			};
		}
	}
}
