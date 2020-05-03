#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_GRAPHICS_SHADER_H
#define _DYNAMIK_RENDERER_VULKAN_GRAPHICS_SHADER_H

#include <vulkan/vulkan.h>
#include <string>

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			enum class VulkanGraphicsShaderType {
				ADGR_VULKAN_SHADER_TYPE_VERTEX,
				ADGR_VULKAN_SHADER_TYPE_TESSELLATION,
				ADGR_VULKAN_SHADER_TYPE_GEOMETRY,
				ADGR_VULKAN_SHADER_TYPE_FRAGMENT
			};

			struct VulkanGraphicsShaderInitInfo {
				std::string path;
				VulkanGraphicsShaderType type = VulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;
			};

			class VulkanGraphicsShader {
			public:
				VulkanGraphicsShader() {}
				~VulkanGraphicsShader() {}

				void initialize(VkDevice logicalDevice, VulkanGraphicsShaderInitInfo info);
				void terminate(VkDevice logicalDevice);

				static VkShaderModule createShaderModule(VkDevice logicalDevice, ARRAY<CHR> code);

				ARRAY<CHR> code;
				VkShaderModule shaderModule = VK_NULL_HANDLE;
				VulkanGraphicsShaderType type;
				VkPipelineShaderStageCreateInfo stageCreateInfo;

			private:
				ARRAY<CHR> getCode(std::string path);
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_SHADER_H
