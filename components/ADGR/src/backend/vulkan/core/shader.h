#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_SHADER_H
#define _DYNAMIK_ADGR_VULKAN_SHADER_H

#include "backend/vulkan/core/data structures/vulkan.h"

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

				void loadShader(DMK_ShaderCode shaderCode, ShaderType type) ;
				VkShaderModule createShaderModule(VkDevice device, DMK_ShaderCode& code) ;
				void deleteShaders(ADGRVulkanDataContainer& container, ADGRVulkanPipelineDataContainer& pipelineContainer);

				void compileShaders(std::string path, bool activate) ;

			private:
				std::string command = "";
				std::string shaderCompilerPath = "E:/Programming/SDKs/Vulkan/1.1.121.0/Bin/glslangValidator.exe";
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SHADER_H
