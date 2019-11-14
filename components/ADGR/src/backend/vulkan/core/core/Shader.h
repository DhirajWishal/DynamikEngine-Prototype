#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_SHADER_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_SHADER_H

#include "Core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			enum ShaderType {
				VERTEX_SHADER,
				TESSELLATION_SHADER,
				GEOMETRY_SHADER,
				FRAGMENT_SHADER
			};

			class Shader : public Core {
			public:
				Shader() {}
				virtual ~Shader() {}

				virtual void loadShader(DMK_ShaderCode shaderCode, ShaderType type) {}
				virtual VkShaderModule createShaderModule(VkDevice device, DMK_ShaderCode& code) { return nullptr; }
				virtual void deleteShaders(ADGRVulkanDataContainer& container, ADGRVulkanPipelineDataContainer& pipelineContainer) {}

				virtual void compileShaders(std::string path, bool activate) {}

			private:
				std::string vertShaderPath;
				std::string fragShaderPath;
#ifdef USE_SHADER_TESSELLATION
				std::string tessShaderPath;
#endif
#ifdef USE_SHADER_GEOMETRY
				std::string geoShaderPath;
#endif
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDERER_SHADER_H
