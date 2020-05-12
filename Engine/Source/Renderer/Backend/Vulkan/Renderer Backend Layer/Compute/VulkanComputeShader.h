#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_COMPUTE_SHADER_H
#define _DYNAMIK_RENDERER_VULKAN_COMPUTE_SHADER_H

#include "../Graphics/VulkanGraphicsShader.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			class VulkanComputeShader : public VulkanGraphicsShader {
			public:
				VulkanComputeShader() {}
				virtual ~VulkanComputeShader() {}

				void initialize(VkDevice logicalDevice, std::string path);
				void terminate(VkDevice logicalDevice);

				static VkShaderModule createShaderModule(VkDevice logicalDevice, std::vector<CHR> code);

				std::vector<CHR> code;
				VkShaderModule shaderModule = VK_NULL_HANDLE;
				VulkanGraphicsShaderType type;
				VkPipelineShaderStageCreateInfo stageCreateInfo;

			private:
				std::vector<CHR> getCode(std::string path);
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_COMPUTE_SHADER_H
