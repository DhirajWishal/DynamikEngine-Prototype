#include "dmkafx.h"
#include "VulkanGraphicsRenderPass.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			void VulkanGraphicsRenderPass::initialize(VkDevice logicalDevice, VulkanRenderPassInitInfo info)
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
			
			void VulkanGraphicsRenderPass::terminate(VkDevice logicalDevice)
			{
				if (renderPass != VK_NULL_HANDLE)
					vkDestroyRenderPass(logicalDevice, renderPass, nullptr);
			}
		}
	}
}