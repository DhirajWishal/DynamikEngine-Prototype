#pragma once

#include "core/Shader.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class shaderManager : public Shader {
			public:
				shaderManager() {}
				~shaderManager() {}

				void init() override;

				void loadShader(DMK_ShaderCode shaderCode, ShaderType type) override;
				VkShaderModule createShaderModule(DMK_ShaderCode& code) override;
				void deleteShaders() override;

			private:
				VkDevice* m_device = &device;

				DMK_ShaderCode* m_vertexShaderCode = &vertexShaderCode;
				VkShaderModule* m_vertexShaderModule = &vertexShaderModule;
				VkPipelineShaderStageCreateInfo* vertShaderStageInfo = &vertexShaderStageInfo;

				DMK_ShaderCode* m_fragmentShaderCode = &fragmentShaderCode;
				VkShaderModule* m_fragmentShaderModule = &fragmentShaderModule;
				VkPipelineShaderStageCreateInfo* fragShaderStageInfo = &fragmentShaderStageInfo;

#ifdef USE_SHADER_TESSELLATION
				DMK_ShaderCode* m_tessellationShaderCode = &tessellationShaderCode;
				VkShaderModule* m_tessellationShaderModule = &tessellationShaderModule;
				VkPipelineShaderStageCreateInfo* tessShaderStageInfo = &tessellationShaderStageInfo;
#endif

#ifdef USE_SHADER_GEOMETRY
				DMK_ShaderCode* m_geometryShaderCode = &geometryShaderCode;
				VkShaderModule* m_geometryShaderModule = &geometryShaderModule;
				VkPipelineShaderStageCreateInfo* geoShaderStageInfo = &geometryShaderStageInfo;
#endif
			};
		}
	}
}
