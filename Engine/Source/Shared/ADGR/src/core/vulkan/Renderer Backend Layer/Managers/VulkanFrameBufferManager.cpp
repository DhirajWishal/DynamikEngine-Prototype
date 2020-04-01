#include "adgrafx.h"
#include "VulkanFrameBufferManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanFrameBufferManager::initialize(POINTER<VulkanCoreObject> core, ADGRVulkanFrameBufferInitInfo info)
			{
				core->frameBuffers.resize(core->swapChainImageViews.size());

				for (size_t i = 0; i < core->swapChainImageViews.size(); i++)
				{
					ARRAY<VkImageView> attachments;
					for (VkImageView _imageView : info.preAttachments)
						attachments.push_back(_imageView);

					attachments.push_back(core->swapChainImageViews[i]);

					for (VkImageView _imageView : info.additionalAttachments)
						attachments.push_back(_imageView);

					VkFramebufferCreateInfo framebufferInfo = {};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = core->renderPass;
					framebufferInfo.attachmentCount = static_cast<UI32>(attachments.size());
					framebufferInfo.pAttachments = attachments.data();
					framebufferInfo.width = core->swapChainExtent.width;
					framebufferInfo.height = core->swapChainExtent.height;
					framebufferInfo.layers = 1;

					if (vkCreateFramebuffer(core->logicalDevice, &framebufferInfo, nullptr, &core->frameBuffers[i]) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create framebuffer!");
				}
			}

			void VulkanFrameBufferManager::terminate(POINTER<VulkanCoreObject> core)
			{
				for (VkFramebuffer buffer : core->frameBuffers)
					vkDestroyFramebuffer(core->logicalDevice, buffer, nullptr);

				core->frameBuffers.clear();
			}
		}
	}
}