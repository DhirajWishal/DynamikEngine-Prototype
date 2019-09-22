#pragma once

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
				virtual VkShaderModule createShaderModule(DMK_ShaderCode& code) { return nullptr; }
				virtual void deleteShaders() {}

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
