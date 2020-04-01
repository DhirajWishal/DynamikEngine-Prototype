#include "adgrafx.h"
#include "VulkanRenderPassManager.h"

#include "VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanRenderPassManager::initialize(POINTER<VulkanCoreObject> core, ADGRVulkanRenderPassInitInfo info)
			{
				ARRAY<VkAttachmentDescription> attachments;

				// attachment descriptions
				VkAttachmentDescription colorAttachment = {};
				colorAttachment.format = core->swapChainImageFormat;
				colorAttachment.samples = core->msaaSamples;
				colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				attachments.push_back(colorAttachment);

				VkAttachmentReference colorAttachmentRef = {};
				colorAttachmentRef.attachment = 0;
				colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

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
					depthAttachment.format = VulkanFunctions::findDepthFormat(core->physicalDevice);
					depthAttachment.samples = core->msaaSamples;
					depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
					depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
					attachments.push_back(depthAttachment);

					VkAttachmentReference depthAttachmentRef = {};
					depthAttachmentRef.attachment = 1;
					depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

					subpass.pDepthStencilAttachment = &depthAttachmentRef;
				}

				VkAttachmentDescription colorAttachmentResolve = {};
				colorAttachmentResolve.format = core->swapChainImageFormat;
				colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
				colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				attachments.push_back(colorAttachmentResolve);

				ARRAY<VkSubpassDescription> subPasses;
				subPasses.push_back(subpass);

				for (VkSubpassDescription _description : info.additionalSubPasses)
					subPasses.push_back(_description);

				VkSubpassDependency dependency = {};
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstSubpass = info.destinationSubpass;
				dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.srcAccessMask = info.accessFlags;
				dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

				ARRAY<VkSubpassDependency> dependencies;
				dependencies.push_back(dependency);

				for (VkSubpassDependency _dependency : info.additionalSubPassDependencies)
					dependencies.push_back(_dependency);

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
				if (vkCreateRenderPass(core->logicalDevice, &renderPassInfo, nullptr, &core->renderPass) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create render pass!");
			}

			void VulkanRenderPassManager::terminate(POINTER<VulkanCoreObject> core)
			{
				if (core->renderPass != VK_NULL_HANDLE)
					vkDestroyRenderPass(core->logicalDevice, core->renderPass, nullptr);
			}
		}
	}
}