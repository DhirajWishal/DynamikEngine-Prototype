#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_GRAPHICS_RENDER_PASS_H
#define _DYNAMIK_RENDERER_VULKAN_GRAPHICS_RENDER_PASS_H

#include <vulkan/vulkan.h>
#include "Public/Array.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			struct VulkanRenderPassInitInfo {
				UI32 destinationSubpass = 0;
				VkAccessFlags accessFlags = 0;
				B1 overrideDependencies = false;

				ARRAY<VkAttachmentDescription> attachments;

				ARRAY<VkSubpassDescription> subPasses;
				ARRAY<VkSubpassDependency> additionalSubPassDependencies;
			};

			class VulkanGraphicsRenderPass {
			public:
				VulkanGraphicsRenderPass() {}
				virtual ~VulkanGraphicsRenderPass() {}

				virtual void initialize(VkDevice logicalDevice, VulkanRenderPassInitInfo info);
				virtual void terminate(VkDevice logicalDevice);

				VkRenderPass renderPass = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_GRAPHICS_RENDER_PASS_H
