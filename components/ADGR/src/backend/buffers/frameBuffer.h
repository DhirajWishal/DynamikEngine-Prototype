#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "src/backend/interface.h"
#include "core/utils/DMK_DataTypes.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class ADGR_API frameBuffer {
			public:
				frameBuffer(VkDevice* device);
				~frameBuffer() {}

				void initBuffer(std::vector<VkImageView> swapChainImageViews,
					VkRenderPass renderPass, VkExtent2D swapChainExtent,
					VkImageView depthImageView, VkImageView colorImageView);

				void deleteFrameBuffers();

				size_t getFrameBuffersSize() const { return frameBuff.size(); }
				VkFramebuffer getFrame(uint32 pos) const { return frameBuff[pos]; }
				std::vector<VkFramebuffer> getFrameBuffers() const { return frameBuff; }

			private:
				VkDevice* myDevice;

				std::vector<VkFramebuffer> frameBuff;

				size_t mySize = 30;
			};
		}
	}
}
