#pragma once

#ifndef _DYNAMIK_ADGR_RENDERER_FORMAT_H
#define _DYNAMIK_ADGR_RENDERER_FORMAT_H

#include "core/data store/internalFormat.h"

#include <vector>
#include <GLFW/glfw3.h>

namespace Dynamik {
	namespace ADGR {
		class RendererFormat {
		public:
			RendererFormat(InternalFormat* internalFormat) : myInternalFormat(internalFormat) {}
			virtual ~RendererFormat() {}

			// Internal formats
			InternalFormat* myInternalFormat = nullptr;

			// Rendering technology
			DMK_ADGR_RENDERING_TECHNOLOGY myRenderTechnology = DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED;
		};
	}
}

#endif // !_DYNAMIK_ADGR_RENDERER_FORMAT_H
