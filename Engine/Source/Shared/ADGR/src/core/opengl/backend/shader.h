#pragma once
#ifndef _DYNAMIK_ADGR_OPENGL_SHADER_H
#define _DYNAMIK_ADGR_OPENGL_SHADER_H
#ifdef DMK_USE_OPENGL

#include "openglFormat.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			enum class DMKOpengGLShaderType {
				DMK_ADGR_OPENGL_SHADER_TYPE_UNKNOWN,
				DMK_ADGR_OPENGL_SHADER_TYPE_VERTEX,
				DMK_ADGR_OPENGL_SHADER_TYPE_TESSELLATION,
				DMK_ADGR_OPENGL_SHADER_TYPE_GEOMETRY,
				DMK_ADGR_OPENGL_SHADER_TYPE_FRAGMENT
			};

			class shaderManager {
			public:
				shaderManager() {}
				~shaderManager() {}

				void init(unsigned I32* ID, const std::string& path, DMKOpengGLShaderType shaderType);
				void bind(const openglFormat* format);
				void unbind();

				void updateBool(unsigned I32 id, const std::string& name, B1 value) const;
				void updateInt(unsigned I32 id, const std::string& name, I32 value) const;
				void updateFloat(unsigned I32 id, const std::string& name, F32 value) const;

				void setTransform(openglFormat* format, std::deque<DMKEventContainer>& eventContainers) const;
			};
		}
	}
}

#endif // DMK_USE_OPENGL
#endif // !_DYNAMIK_ADGR_OPENGL_SHADER_H
