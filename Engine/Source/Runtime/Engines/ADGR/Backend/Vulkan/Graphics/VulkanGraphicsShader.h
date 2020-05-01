#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_SHADER_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_SHADER_H

#include <vulkan/vulkan.h>
#include <string>

#include "../../Primitives/Shader.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanGraphicsShaderInitInfo {
				std::string path;
				DMKShaderLocation location = DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX;
			};

			class VulkanGraphicsShader : public Shader {
			public:
				VulkanGraphicsShader() {}
				VulkanGraphicsShader(DMKShaderLocation location) : Shader(location) {}
				~VulkanGraphicsShader() {}

				void initialize(VkDevice logicalDevice, ADGRVulkanGraphicsShaderInitInfo info);
				void terminate(VkDevice logicalDevice);

				static VkShaderModule createShaderModule(VkDevice logicalDevice, ARRAY<CHR> code);

				VkShaderModule shaderModule = VK_NULL_HANDLE;
				VkPipelineShaderStageCreateInfo stageCreateInfo;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SHADER_H
