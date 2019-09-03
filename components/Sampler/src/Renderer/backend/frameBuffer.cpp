#include "smpafx.h"
#include "frameBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			void frameBufferManager::init() {
				mySize = static_cast<uint32_t>(swapChainImageViews.size());
				m_frameBuffers->resize(mySize);

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

					if (vkCreateFramebuffer(*m_device, &framebufferInfo, nullptr, &m_frameBuffers->at(i)) != VK_SUCCESS)
						std::runtime_error("failed to create framebuffer!");
				}
			}
			void frameBufferManager::createFrameBuffers(DMKFrameBuffersCreateInfo info) {
				info.frameBuffers->resize(info.swapChainImageViews.size());

				for (size_t i = 0; i < info.swapChainImageViews.size(); i++) {
					std::array<VkImageView, 3> attachments = {
							info.colorImageView,
							info.depthImageView,
							info.swapChainImageViews[i]
					};

					VkFramebufferCreateInfo framebufferInfo = {};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = renderPass;
					framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
					framebufferInfo.pAttachments = attachments.data();
					framebufferInfo.width = swapChainExtent.width;
					framebufferInfo.height = swapChainExtent.height;
					framebufferInfo.layers = 1;

					if (vkCreateFramebuffer(*m_device, &framebufferInfo, nullptr, &info.frameBuffers->at(i)) != VK_SUCCESS)
						std::runtime_error("failed to create framebuffer!");
				}
			}
		}
	}
}