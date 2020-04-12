#include "adgrafx.h"
#include "VulkanSwapChain3D.h"

#include "Objects/VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanSwapChain3D::basicInitialize(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR surfaceCapabilities)
			{
				myPhysicalDevice = physicalDevice;
				swapChainContainer.setBasicData(device, physicalDevice, surface, surfaceCapabilities);
			}

			void VulkanSwapChain3D::initializeSwapChain(UI32 width, UI32 height)
			{
				swapChainContainer.initializeSwapChain(width, height);
			}

			void VulkanSwapChain3D::initializeRenderPass(VkSampleCountFlagBits msaaSamples)
			{
				ARRAY<VkAttachmentDescription> attachments;

				// attachment descriptions
				VkAttachmentDescription colorAttachment = {};
				colorAttachment.format = swapChainContainer.getSwapChainImageFormat();
				colorAttachment.samples = msaaSamples;
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
				depthAttachment.format = VulkanFunctions::findDepthFormat(myPhysicalDevice);
				depthAttachment.samples = msaaSamples;
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
				colorAttachmentResolve.format = swapChainContainer.getSwapChainImageFormat();
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
				swapChainContainer.initializeRenderPass(renderPassInitInfo);
			}

			void VulkanSwapChain3D::initializeFrameBuffers(ADGRVulkanFrameBufferInitInfo frameBufferInitInfo)
			{
				swapChainContainer.initializeFrameBuffer(frameBufferInitInfo);
			}

			void VulkanSwapChain3D::terminate()
			{
				swapChainContainer.terminate();
			}
		}
	}
}