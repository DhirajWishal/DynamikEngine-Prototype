#pragma once
#ifndef _DYNAMIK_ADGR_OPENGL_VERTEX_AND_INDEX_BUFFER_H
#define _DYNAMIK_ADGR_OPENGL_VERTEX_AND_INDEX_BUFFER_H
#ifdef DMK_USE_OPENGL


#include "openglFormat.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class vertexAndIndexBufferManager {
			public:
				vertexAndIndexBufferManager() {}
				~vertexAndIndexBufferManager() {}

				void init(openglFormat* format);
				void bind(const openglFormat* format);
				void draw(const openglFormat* format);
				void clear(const openglFormat* format);
			};
		}
	}
}

#endif // DMK_USE_OPENGL

#endif // !_DYNAMIK_ADGR_OPENGL_VERTEX_AND_INDEX_BUFFER_H
