#pragma once

#ifndef _DYNAMIK_ADGR_OPENGL_TEXTURE_H
#define _DYNAMIK_ADGR_OPENGL_TEXTURE_H
#ifdef DMK_USE_OPENGL

#include "openglFormat.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class textureManager {
			public:
				textureManager() {}
				~textureManager() {}

				void init(openglFormat* format);
				void bind(openglFormat* format, unsigned int slot = 0);
				void unBind();
				void destroy(unsigned int id);
			};
		}
	}
}

#endif // DMK_USE_OPENGL

#endif // !_DYNAMIK_ADGR_OPENGL_TEXTURE_H

