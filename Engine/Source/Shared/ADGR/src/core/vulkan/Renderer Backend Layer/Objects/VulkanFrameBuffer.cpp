#include "adgrafx.h"
#include "VulkanFrameBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanFrameBuffer::initialize(ADGRVulkanFrameBufferInitInfo info)
			{
				frameBuffers.resize(info.swapChain.mySwapChainImageViews.size());

				for (size_t i = 0; i < info.swapChain.mySwapChainImageViews.size(); i++)
				{
					ARRAY<VkImageView> attachments;
					for (VkImageView _imageView : info.preAttachments)
						attachments.pushBack(_imageView);

					attachments.pushBack(info.swapChain.mySwapChainImageViews[i]);

					for (VkImageView _imageView : info.additionalAttachments)
						attachments.pushBack(_imageView);

					VkFramebufferCreateInfo framebufferInfo = {};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = info.renderPass.renderPass;
					framebufferInfo.attachmentCount = static_cast<UI32>(attachments.size());
					framebufferInfo.pAttachments = attachments.data();
					framebufferInfo.width = info.swapChain.mySwapChainExtent.width;
					framebufferInfo.height = info.swapChain.mySwapChainExtent.height;
					framebufferInfo.layers = 1;

					if (vkCreateFramebuffer(info.device.logicalDevice, &framebufferInfo, nullptr, &frameBuffers[i]) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create framebuffer!");
				}
			}

			void VulkanFrameBuffer::terminate(VulkanDevice device)
			{
				for (VkFramebuffer buffer : frameBuffers)
					vkDestroyFramebuffer(device.logicalDevice, buffer, nullptr);

				frameBuffers.clear();
			}
		}
	}
}