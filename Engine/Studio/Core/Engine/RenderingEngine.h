#pragma once
#ifndef _STUDIO_RENDERING_ENGINE_H
#define _STUDIO_RENDERING_ENGINE_H

#include "Dynamik.h"
using namespace Dynamik;
using namespace Dynamik::Renderer;

namespace Studio {
	class RenderingEngine {
	public:
		RenderingEngine() {}
		~RenderingEngine() {}

		void setWindowExtent(UI32 width, UI32 height);
		void setWindowHandle(POINTER<GLFWwindow> windowHandle);
		void createRenderContext(DMKRenderContextType type);

		void initializeStageOne();

	private:
		POINTER<GLFWwindow> windowPointer;
	};
}

#endif // !_STUDIO_RENDERING_ENGINE_H
