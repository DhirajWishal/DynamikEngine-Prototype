#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_PUSH_CONSTANT_H
#define _DYNAMIK_ADGR_VULKAN_PUSH_CONSTANT_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanPushConstant {
			public:
				VulkanPushConstant() {}
				virtual ~VulkanPushConstant() {}

				virtual void pushData(VkCommandBuffer buffer, VkPipelineLayout pipelineLayout);

				UI32 offset = 0;
				UI32 byteSize = 0;
				VkShaderStageFlags stageFlag = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
				VPTR data = nullptr;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_PUSH_CONSTANT_H
