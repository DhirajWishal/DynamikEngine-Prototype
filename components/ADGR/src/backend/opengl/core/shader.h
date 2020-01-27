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

				void init(unsigned int* ID, const std::string& path, DMKOpengGLShaderType shaderType);
				void bind(const openglFormat* format);
				void unbind();

				void updateBool(unsigned int id, const std::string& name, bool value) const;
				void updateInt(unsigned int id, const std::string& name, int value) const;
				void updateFloat(unsigned int id, const std::string& name, float value) const;

				void setTransform(openglFormat* format, std::deque<DMKEventContainer>& eventContainers) const;
			};
		}
	}
}

#endif // DMK_USE_OPENGL
#endif // !_DYNAMIK_ADGR_OPENGL_SHADER_H
