#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_SHADER_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_SHADER_H

#include <vulkan/vulkan.h>
#include <string>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			enum class ADGRVulkanGraphicsShaderType {
				ADGR_VULKAN_SHADER_TYPE_VERTEX,
				ADGR_VULKAN_SHADER_TYPE_TESSELLATION,
				ADGR_VULKAN_SHADER_TYPE_GEOMETRY,
				ADGR_VULKAN_SHADER_TYPE_FRAGMENT
			};

			struct ADGRVulkanGraphicsShaderInitInfo {
				std::string path;
				ADGRVulkanGraphicsShaderType type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;
			};

			class VulkanGraphicsShader {
			public:
				VulkanGraphicsShader() {}
				~VulkanGraphicsShader() {}

				void initialize(VkDevice logicalDevice, ADGRVulkanGraphicsShaderInitInfo info);
				void terminate(VkDevice logicalDevice);

				static VkShaderModule createShaderModule(VkDevice logicalDevice, ARRAY<CHR> code);

				ARRAY<CHR> code;
				VkShaderModule shaderModule = VK_NULL_HANDLE;
				ADGRVulkanGraphicsShaderType type;
				VkPipelineShaderStageCreateInfo stageCreateInfo;

			private:
				ARRAY<CHR> getCode(std::string path);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SHADER_H
