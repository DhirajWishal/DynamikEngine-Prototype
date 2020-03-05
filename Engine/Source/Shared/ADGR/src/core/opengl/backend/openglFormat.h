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

				unsigned I32 myShaderID = 0;

				unsigned I32 vertexArrayObject = 0;
				unsigned I32 vertexBufferObject = 0;
				unsigned I32 indexBufferObject = 0;	// elementBufferObject

				std::vector<unsigned I32> myTextureIDs = {};
			};
		}
	}
}
#endif // DMK_USE_OPENGL

#endif // _DYNAMIK_ADGR_OPENGL_FORMAT_H
