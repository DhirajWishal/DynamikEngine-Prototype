#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GLOBAL_VARIABLES_H
#define _DYNAMIK_ADGR_VULKAN_GLOBAL_VARIABLES_H

#include <vulkan/vulkan.h>
#include "VulkanPushConstant.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			/*
			 TODO:
			 * Remove vulkan format and just have a container for index and vertex buffers (vulkan side)
			 * and render them accordingly.
			 * Same for uniform buffers.
			 */
			class VulkanGlobalVariables {
			public:
				VulkanGlobalVariables() {}
				~VulkanGlobalVariables() {}

				VkSampleCountFlagBits msaaSamples;

				//VulkanPushConstant pushConstant;
				ARRAY<glm::vec4> pushConstants;

				UI32 minMipLevel;
				UI32 maxMipLevel;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_GLOBAL_VARIABLES_H
