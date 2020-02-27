#include "adgrafx.h"
#include "frameBufferManager.h"

#ifdef DMK_USE_VULKAN 
namespace Dynamik {
	namespace ADGR {
		namespace core {
			void frameBufferManager::init(ADGRVulkanDataContainer* container) {
				mySize = static_cast<UI32>(container->swapchainContainer.swapchainImageViews.size());
				container->frameBufferContainer.buffers.resize(mySize);

				for (size_t i = 0; i < mySize; i++) {
					std::array<VkImageView, 3> attachments = {
							container->colorBufferContainer.imageView,
							container->depthBufferContainer.imageView,
							container->swapchainContainer.swapchainImageViews[i]
					};

					VkFramebufferCreateInfo framebufferInfo = {};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = container->renderPass;
					framebufferInfo.attachmentCount = static_cast<UI32>(attachments.size());
					framebufferInfo.pAttachments = attachments.data();
					framebufferInfo.width = container->swapchainContainer.swapchainExtent.width;
					framebufferInfo.height = container->swapchainContainer.swapchainExtent.height;
					framebufferInfo.layers = 1;

					if (vkCreateFramebuffer(container->device, &framebufferInfo, nullptr, &container->frameBufferContainer.buffers[i]) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create framebuffer!");
				}
			}
			void frameBufferManager::createFrameBuffers(ADGRVulkanDataContainer* container, DMKFrameBuffersCreateInfo info) {
				info.frameBuffers->resize(info.swapChainImageViews.size());

				for (size_t i = 0; i < info.swapChainImageViews.size(); i++) {
					std::array<VkImageView, 3> attachments = {
							info.colorImageView,
							info.depthImageView,
							info.swapChainImageViews[i]
					};

					VkFramebufferCreateInfo framebufferInfo = {};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = container->renderPass;
					framebufferInfo.attachmentCount = static_cast<UI32>(attachments.size());
					framebufferInfo.pAttachments = attachments.data();
					framebufferInfo.width = container->swapchainContainer.swapchainExtent.width;
					framebufferInfo.height = container->swapchainContainer.swapchainExtent.height;
					framebufferInfo.layers = 1;

					if (vkCreateFramebuffer(container->device, &framebufferInfo, nullptr, &info.frameBuffers->at(i)) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create framebuffer!");
				}
			}

			void frameBufferManager::clear(ADGRVulkanDataContainer* container) {
				// destroy frame buffers
				for (VkFramebuffer buffer : container->frameBufferContainer.buffers)
					vkDestroyFramebuffer(container->device, buffer, nullptr);

				container->frameBufferContainer.buffers.clear();
			}
		}
	}
}
#endif // DMK_USE_VULKAN