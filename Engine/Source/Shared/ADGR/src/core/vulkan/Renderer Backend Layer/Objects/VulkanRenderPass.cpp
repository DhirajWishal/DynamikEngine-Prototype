#include "adgrafx.h"
#include "VulkanRenderPass.h"
#include "VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanRenderPass::initialize(ADGRVulkanRenderPassInitInfo info)
			{
				ARRAY<VkAttachmentDescription> attachments;

				// attachment descriptions
				VkAttachmentDescription colorAttachment = {};
				colorAttachment.format = info.swapChain.mySwapChainImageFormat;
				colorAttachment.samples = info.global.msaaSamples;
				colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				attachments.pushBack(colorAttachment);

				VkAttachmentReference colorAttachmentRef = {};
				colorAttachmentRef.attachment = 0;
				colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				VkAttachmentDescription colorAttachmentResolve = {};
				colorAttachmentResolve.format = info.swapChain.mySwapChainImageFormat;
				colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
				colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				attachments.pushBack(colorAttachmentResolve);

				VkAttachmentReference colorAttachmentResolveRef = {};
				colorAttachmentResolveRef.attachment = 2;
				colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				// sub passes
				VkSubpassDescription subpass = {};
				subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpass.colorAttachmentCount = 1;
				subpass.pColorAttachments = &colorAttachmentRef;
				subpass.pResolveAttachments = &colorAttachmentResolveRef;

				if (info.enableDepthAttachment)
				{
					VkAttachmentDescription depthAttachment = {};
					depthAttachment.format = VulkanFunctions::findDepthFormat(info.device.physicalDevice);
					depthAttachment.samples = info.global.msaaSamples;
					depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
					depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
					attachments.pushBack(depthAttachment);

					VkAttachmentReference depthAttachmentRef = {};
					depthAttachmentRef.attachment = 1;
					depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

					subpass.pDepthStencilAttachment = &depthAttachmentRef;
				}

				ARRAY<VkSubpassDescription> subPasses;
				subPasses.pushBack(subpass);

				for (VkSubpassDescription _description : info.additionalSubPasses)
					subPasses.pushBack(_description);

				VkSubpassDependency dependency = {};
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstSubpass = info.destinationSubpass;
				dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.srcAccessMask = info.accessFlags;
				dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

				ARRAY<VkSubpassDependency> dependencies;
				dependencies.pushBack(dependency);

				for (VkSubpassDependency _dependency : info.additionalSubPassDependencies)
					dependencies.pushBack(_dependency);

				// render pass info
				VkRenderPassCreateInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				renderPassInfo.attachmentCount = static_cast<UI32>(attachments.size());
				renderPassInfo.pAttachments = attachments.data();
				renderPassInfo.subpassCount = subPasses.size();
				renderPassInfo.pSubpasses = subPasses.data();
				renderPassInfo.dependencyCount = dependencies.size();
				renderPassInfo.pDependencies = dependencies.data();

				// create the render pass
				if (vkCreateRenderPass(info.device.logicalDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create render pass!");
			}

			void VulkanRenderPass::terminate(VulkanDevice device)
			{
				if (renderPass != VK_NULL_HANDLE)
					vkDestroyRenderPass(device.logicalDevice, renderPass, nullptr);
			}
		}
	}
}