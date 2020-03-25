#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_SHADER_H
#define _DYNAMIK_ADGR_VULKAN_SHADER_H

#include <string>
#include "VulkanDevice.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			enum class ADGRVulkanShaderType {
				ADGR_VULKAN_SHADER_TYPE_VERTEX,
				ADGR_VULKAN_SHADER_TYPE_TESSELLATION,
				ADGR_VULKAN_SHADER_TYPE_GEOMETRY,
				ADGR_VULKAN_SHADER_TYPE_FRAGMENT
			};

			struct ADGRVulkanShaderInitInfo {
				VulkanDevice device;
				
				std::string path;
				ADGRVulkanShaderType type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;
			};

			class VulkanShader {
			public:
				VulkanShader() {}
				~VulkanShader() {}

				void initialize(ADGRVulkanShaderInitInfo info);
				void terminate(VulkanDevice device);

				ARRAY<CHR> code;
				VkShaderModule shaderModule = VK_NULL_HANDLE;
				ADGRVulkanShaderType type;
				VkPipelineShaderStageCreateInfo stageCreateInfo;

			private:
				ARRAY<CHR> getCode(std::string path);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SHADER_H
