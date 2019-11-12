#pragma once

#include "core/Shader.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class shaderManager : public Shader {
			public:
				shaderManager() {}
				~shaderManager() {}

				void init(ADGRVulkanDataContainer* container, ADGRVulkanShaderDataContainer* shaderDataContainer);

				void loadShader(DMK_ShaderCode shaderCode, ShaderType type) override;
				VkShaderModule createShaderModule(VkDevice device, DMK_ShaderCode& code) override;
				void deleteShaders(ADGRVulkanDataContainer& container, ADGRVulkanPipelineDataContainer& pipelineContainer) override;

				void compileShaders(std::string path, bool activate) override;

			private:
				std::string command = "";
				std::string shaderCompilerPath = "E:/Programming/SDKs/Vulkan/1.1.121.0/Bin/glslangValidator.exe";
			};
		}
	}
}
