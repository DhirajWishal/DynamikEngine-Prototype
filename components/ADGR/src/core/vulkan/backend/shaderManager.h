#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_SHADER_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_SHADER_MANAGER_H
#ifdef DMK_USE_VULKAN

#include "core/vulkan/backend/data structures/vulkan.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			enum ShaderType {
				VERTEX_SHADER,
				TESSELLATION_SHADER,
				GEOMETRY_SHADER,
				FRAGMENT_SHADER
			};

			class shaderManager {
			public:
				shaderManager() {}
				~shaderManager() {}

				void init(ADGRVulkanDataContainer* container, ADGRVulkanShaderDataContainer* shaderDataContainer);

				void loadShader(DMK_ShaderCode shaderCode, ShaderType type);
				VkShaderModule createShaderModule(VkDevice device, DMK_ShaderCode& code);
				void deleteShaders(ADGRVulkanDataContainer& container, ADGRVulkanPipelineDataContainer& pipelineContainer);

				void compileShaders(std::string path, B1 activate);

			private:
				std::string command = "";
				std::string shaderCompilerPath = "E:/Programming/SDKs/Vulkan/1.1.121.0/Bin/glslangValidator.exe";
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_SHADER_H
