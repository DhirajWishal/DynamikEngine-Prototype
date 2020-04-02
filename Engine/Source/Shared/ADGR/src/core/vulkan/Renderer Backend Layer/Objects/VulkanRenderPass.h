#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_RENDER_PASS_H
#define _DYNAMIK_ADGR_VULKAN_RENDER_PASS_H

#include "VulkanSwapChain.h"
#include "VulkanGlobalVariables.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanRenderPassInitInfo {
				VulkanDevice device;
				VulkanSwapChain swapChain;
				VulkanGlobalVariables global;

				B1 enableDepthAttachment = true;

				UI32 destinationSubpass = 0;
				VkAccessFlags accessFlags = 0;

				ARRAY<VkSubpassDescription> additionalSubPasses;
				ARRAY<VkSubpassDependency> additionalSubPassDependencies;
			};

			class VulkanRenderPass {
			public:
				VulkanRenderPass() {}
				~VulkanRenderPass() {}

				void initialize(ADGRVulkanRenderPassInitInfo info);
				void terminate(VulkanDevice device);

				VkRenderPass renderPass = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDER_PASS_H
