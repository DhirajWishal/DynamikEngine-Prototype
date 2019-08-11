/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "smpafx.h"
#include "frameBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			frameBuffer::frameBuffer(VkDevice* device) :
				myDevice(device) {
			}

			void frameBuffer::initBuffer(std::vector<VkImageView> swapChainImageViews,
				VkRenderPass renderPass, VkExtent2D swapChainExtent, VkImageView depthImageView,
				VkImageView colorImageView) {
				mySize = swapChainImageViews.size();
				frameBuff.resize(mySize);

				for (size_t i = 0; i < mySize; i++) {
					std::array<VkImageView, 3> attachments = {
							colorImageView,
							depthImageView,
							swapChainImageViews[i]
					};

					VkFramebufferCreateInfo framebufferInfo = {};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = renderPass;
					framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
					framebufferInfo.pAttachments = attachments.data();
					framebufferInfo.width = swapChainExtent.width;
					framebufferInfo.height = swapChainExtent.height;
					framebufferInfo.layers = 1;

					if (vkCreateFramebuffer(*myDevice, &framebufferInfo, nullptr, &frameBuff[i]) != VK_SUCCESS)
						throw std::runtime_error("failed to create framebuffer!");
				}
			}

			void frameBuffer::deleteFrameBuffers() {
				for (auto framebuffer : frameBuff) {
					vkDestroyFramebuffer(*myDevice, framebuffer, nullptr);
				}
			}
		}
	}
}
