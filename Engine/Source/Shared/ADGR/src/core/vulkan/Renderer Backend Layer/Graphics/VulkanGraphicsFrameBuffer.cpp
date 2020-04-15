#include "adgrafx.h"
#include "VulkanGraphicsFrameBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanGraphicsFrameBuffer::initializeRenderPass(VkDevice logicalDevice, ADGRVulkanRenderPassInitInfo info)
			{
				ARRAY<VkSubpassDependency> dependencies;

				if (!info.overrideDependencies)
				{
					VkSubpassDependency dependency = {};
					dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
					dependency.dstSubpass = info.destinationSubpass;
					dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					dependency.srcAccessMask = info.accessFlags;
					dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

					dependencies.push_back(dependency);
				}

				for (VkSubpassDependency _dependency : info.additionalSubPassDependencies)
					dependencies.push_back(_dependency);

				// render pass info
				VkRenderPassCreateInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				renderPassInfo.attachmentCount = static_cast<UI32>(info.attachments.size());
				renderPassInfo.pAttachments = info.attachments.data();
				renderPassInfo.subpassCount = info.subPasses.size();
				renderPassInfo.pSubpasses = info.subPasses.data();
				renderPassInfo.dependencyCount = dependencies.size();
				renderPassInfo.pDependencies = dependencies.data();

				// create the render pass
				if (vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create render pass!");
			}

			void VulkanGraphicsFrameBuffer::initializeFrameBuffer(VkDevice logicalDevice, ADGRVulkanGraphicsFrameBufferInitInfo info)
			{
				frameHeight = info.swapChainExtent.height;
				frameWidth = info.swapChainExtent.width;

				for (size_t i = 0; i < info.bufferCount; i++)
				{
					ARRAY<VkImageView> attachments;
					for (VkImageView _imageView : info.attachments)
						attachments.push_back(_imageView);

					if ((info.swapChainImageViews[i] != VK_NULL_HANDLE) && (info.swapChainImageViews.size()))
						attachments.pushBack(info.swapChainImageViews[i]);

					VkFramebufferCreateInfo framebufferInfo = {};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = renderPass;
					if (info.overrideRenderPass != VK_NULL_HANDLE)
					{
						framebufferInfo.renderPass = info.overrideRenderPass;
						renderPass = info.overrideRenderPass;
					}
					framebufferInfo.attachmentCount = static_cast<UI32>(attachments.size());
					framebufferInfo.pAttachments = attachments.data();
					framebufferInfo.width = info.swapChainExtent.width;
					framebufferInfo.height = info.swapChainExtent.height;
					framebufferInfo.layers = 1;

					VkFramebuffer _buffer = VK_NULL_HANDLE;
					if (vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &_buffer) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create framebuffer!");

					buffers.pushBack(_buffer);
				}
			}

			void VulkanGraphicsFrameBuffer::terminate(VkDevice logicalDevice)
			{
				if (renderPass != VK_NULL_HANDLE)
					vkDestroyRenderPass(logicalDevice, renderPass, nullptr);

				for (VkFramebuffer buffer : buffers)
					vkDestroyFramebuffer(logicalDevice, buffer, nullptr);

				buffers.clear();
			}

			VkFramebuffer VulkanGraphicsFrameBuffer::createFrameBuffer(
				VkDevice logicalDevice,
				VkRenderPass renderPass,
				ARRAY<VkImageView> attachments,
				VkExtent2D extent,
				UI32 layerCount)
			{
				VkFramebufferCreateInfo framebufferInfo = {};
				framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass = renderPass;
				framebufferInfo.attachmentCount = attachments.size();
				framebufferInfo.pAttachments = attachments.data();
				framebufferInfo.width = extent.width;
				framebufferInfo.height = extent.height;
				framebufferInfo.layers = layerCount;

				VkFramebuffer _buffer = VK_NULL_HANDLE;
				if (vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &_buffer) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create framebuffer!");

				return _buffer;
			}

			void VulkanGraphicsFrameBuffer::terminateFrameBuffer(VkDevice logicalDevice, VkFramebuffer buffer)
			{
				vkDestroyFramebuffer(logicalDevice, buffer, nullptr);
			}
		}
	}
}