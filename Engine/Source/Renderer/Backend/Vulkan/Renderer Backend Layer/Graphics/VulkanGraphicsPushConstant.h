#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_GRAPHICS_PUSH_CONSTANT_H
#define _DYNAMIK_RENDERER_VULKAN_GRAPHICS_PUSH_CONSTANT_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			class VulkanGraphicsPushConstant {
			public:
				VulkanGraphicsPushConstant() {}
				virtual ~VulkanGraphicsPushConstant() {}

				virtual void pushData(VkCommandBuffer buffer, VkPipelineLayout pipelineLayout);

				UI32 offset = 0;
				UI32 byteSize = 0;
				VkShaderStageFlags stageFlag = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
				VPTR data = nullptr;
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_PUSH_CONSTANT_H
