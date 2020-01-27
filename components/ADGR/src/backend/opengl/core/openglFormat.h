#pragma once

#ifndef _DYNAMIK_ADGR_OPENGL_FORMAT_H
#define _DYNAMIK_ADGR_OPENGL_FORMAT_H

#include "rendererFormat.h"

#ifdef DMK_USE_OPENGL
namespace Dynamik {
	namespace ADGR {
		namespace core {
			class openglFormat {
			public:
				openglFormat(RendererFormat* format) : myRendererFormat(format) {}
				~openglFormat() {}

				// local renderer format
				RendererFormat* myRendererFormat = nullptr;

				unsigned int myShaderID = 0;

				unsigned int vertexArrayObject = 0;
				unsigned int vertexBufferObject = 0;
				unsigned int indexBufferObject = 0;	// elementBufferObject

				std::vector<unsigned int> myTextureIDs = {};
			};
		}
	}
}
#endif // DMK_USE_OPENGL

#endif // _DYNAMIK_ADGR_OPENGL_FORMAT_H

