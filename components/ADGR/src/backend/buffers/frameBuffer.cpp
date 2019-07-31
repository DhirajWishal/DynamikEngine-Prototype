/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "frameBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			frameBuffer::frameBuffer() {
			}

			void frameBuffer::initBuffer(std::vector<VkImageView> swapChainImageViews,
				VkRenderPass renderPass, VkExtent2D swapChainExtent) {
				mySize = swapChainImageViews.size();
				myFrameBuffer.resize(mySize);
				for (size_t i = 0; i < mySize; i++) {
					VkImageView attachments[] = {
						swapChainImageViews[i]
					};

					VkFramebufferCreateInfo framebufferInfo = {};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = renderPass;
					framebufferInfo.attachmentCount = 1;
					framebufferInfo.pAttachments = attachments;
					framebufferInfo.width = swapChainExtent.width;
					framebufferInfo.height = swapChainExtent.height;
					framebufferInfo.layers = 1;

					if (vkCreateFramebuffer(*myDevice, &framebufferInfo, nullptr, &myFrameBuffer[i]) != VK_SUCCESS)
						throw std::runtime_error("failed to create framebuffer!");
				}
			}

			void frameBuffer::deleteBuffers() {
				for (auto framebuffer : myFrameBuffer)
					vkDestroyFramebuffer(*myDevice, framebuffer, nullptr);
			}
		}
	}
}
