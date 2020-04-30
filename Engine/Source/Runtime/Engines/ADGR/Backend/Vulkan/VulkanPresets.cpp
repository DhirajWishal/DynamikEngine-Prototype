#include "dmkafx.h"
#include "VulkanPresets.h"

#include "Common/VulkanUtilities.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			ADGRVulkanRenderPassInitInfo VulkanPresets::renderPassPreset3D(VkFormat swapChainImageFormat, VkFormat depthFormat, VkSampleCountFlagBits msaaFlags)
			{
				ARRAY<VkAttachmentDescription> attachments;

				// attachment descriptions
				VkAttachmentDescription colorAttachment = {};
				colorAttachment.format = swapChainImageFormat;
				colorAttachment.samples = msaaFlags;
				colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				attachments.push_back(colorAttachment);

				VkAttachmentReference colorAttachmentRef = {};
				colorAttachmentRef.attachment = 0;
				colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				// sub passes
				VkSubpassDescription subpass = {};
				subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpass.colorAttachmentCount = 1;
				subpass.pColorAttachments = &colorAttachmentRef;

				VkAttachmentDescription depthAttachment = {};
				depthAttachment.format = depthFormat;
				depthAttachment.samples = msaaFlags;
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

				VkAttachmentDescription colorAttachmentResolve = {};
				colorAttachmentResolve.format = swapChainImageFormat;
				colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
				colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				attachments.push_back(colorAttachmentResolve);

				VkAttachmentReference colorAttachmentResolveRef = {};
				colorAttachmentResolveRef.attachment = 2;
				colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				subpass.pResolveAttachments = &colorAttachmentResolveRef;

				ARRAY<VkSubpassDescription> subPasses;
				subPasses.push_back(subpass);

				ADGRVulkanRenderPassInitInfo renderPassInitInfo;
				renderPassInitInfo.attachments = attachments;
				renderPassInitInfo.subPasses = subPasses;
				return renderPassInitInfo;
			}

			ADGRVulkanRenderPassInitInfo VulkanPresets::renderPassPresetOverlay(VkFormat swapChainImageFormat, VkFormat depthFormat)
			{
				VkAttachmentDescription attachments[2] = {};

				// Color attachment
				attachments[0].format = swapChainImageFormat;
				attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
				// Don't clear the framebuffer (like the renderpass from the example does)
				attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
				attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[0].initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

				// Depth attachment
				attachments[1].format = depthFormat;
				attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
				attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				VkAttachmentReference colorReference = {};
				colorReference.attachment = 0;
				colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				VkAttachmentReference depthReference = {};
				depthReference.attachment = 1;
				depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				VkSubpassDescription subpass = {};
				subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpass.colorAttachmentCount = 1;
				subpass.pColorAttachments = &colorReference;
				subpass.pDepthStencilAttachment = &depthReference;

				// Use subpass dependencies for image layout transitions
				VkSubpassDependency subpassDependencies[2] = {};

				// Transition from final to initial (VK_SUBPASS_EXTERNAL refers to all commmands executed outside of the actual renderpass)
				subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependencies[0].dstSubpass = 0;
				subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				subpassDependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				// Transition from initial to final
				subpassDependencies[1].srcSubpass = 0;
				subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				subpassDependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				subpassDependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				ARRAY<VkSubpassDescription> subPasses;
				subPasses.push_back(subpass);

				ADGRVulkanRenderPassInitInfo renderPassInitInfo;
				renderPassInitInfo.attachments = { attachments[0], attachments[1] };
				renderPassInitInfo.subPasses = subPasses;
				renderPassInitInfo.additionalSubPassDependencies = { subpassDependencies[0], subpassDependencies[1] };
				renderPassInitInfo.overrideDependencies = true;
				return renderPassInitInfo;
			}
			
			ADGRVulkanGraphicsFrameBufferInitInfo VulkanPresets::frameBufferPreset3D(VkImageView colorImage, VkImageView depthImage, ARRAY<VkImageView> swapChainImageViews, VkExtent2D bufferExtent, UI32 bufferCount)
			{
				ADGRVulkanGraphicsFrameBufferInitInfo frameBufferInitInfo;
				frameBufferInitInfo.attachments.pushBack(colorImage);
				frameBufferInitInfo.attachments.pushBack(depthImage);
				frameBufferInitInfo.swapChainImageViews = swapChainImageViews;
				frameBufferInitInfo.swapChainExtent = bufferExtent;
				frameBufferInitInfo.bufferCount = bufferCount;
				return frameBufferInitInfo;
			}
			
			ADGRVulkanGraphicsPipelineInitInfo VulkanPresets::pipelinePreset3D(ARRAY<DMKVertexAttribute> attributes, ShaderPaths shaderPaths)
			{
				//auto _shaders = VulkanUtilities::getGraphicsShaders()

				return ADGRVulkanGraphicsPipelineInitInfo();
			}
		}
	}
}