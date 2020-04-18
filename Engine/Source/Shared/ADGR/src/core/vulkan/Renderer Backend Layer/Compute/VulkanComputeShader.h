#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_COMPUTE_SHADER_H
#define _DYNAMIK_ADGR_VULKAN_COMPUTE_SHADER_H

#include "../Graphics/VulkanGraphicsShader.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanComputeShader : public VulkanGraphicsShader {
			public:
				VulkanComputeShader() {}
				virtual ~VulkanComputeShader() {}

				void initialize(VkDevice logicalDevice, std::string path);
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

#endif // !_DYNAMIK_ADGR_VULKAN_COMPUTE_SHADER_H
