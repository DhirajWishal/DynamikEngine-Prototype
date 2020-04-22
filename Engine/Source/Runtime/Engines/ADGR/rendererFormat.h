#pragma once

#ifndef _DYNAMIK_ADGR_RENDERER_FORMAT_H
#define _DYNAMIK_ADGR_RENDERER_FORMAT_H

#include "Objects/InternalFormat/InteralFormat.h"

#include <vector>
#include <GLFW/glfw3.h>

namespace Dynamik {
	namespace ADGR {
		class RendererFormat {
		public:
			RendererFormat() {}
			RendererFormat(POINTER<InternalFormat> internalFormat) : myInternalFormat(internalFormat) {}
			virtual ~RendererFormat() {}

			// Internal formats
			POINTER<InternalFormat> myInternalFormat;

			// Rendering technology
			DMKRenderingTechnology myRenderTechnology = DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_INDEXED;
		};
	}
}

#endif // !_DYNAMIK_ADGR_RENDERER_FORMAT_H
