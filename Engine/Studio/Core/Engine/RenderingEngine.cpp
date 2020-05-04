#include "studioafx.h"
#include "RenderingEngine.h"

namespace Studio {
	void RenderingEngine::setWindowExtent(UI32 width, UI32 height)
	{
		DMKRenderer::setWindowExtent(width, height);
	}
	
	void RenderingEngine::setWindowHandle(POINTER<GLFWwindow> windowHandle)
	{
		DMKRenderer::setWindowHandle(windowHandle);
		windowPointer = windowHandle;
	}
	
	void RenderingEngine::createRenderContext(DMKRenderContextType type)
	{
		DMKRenderer::createNewContext(windowPointer, type);
	}
	
	void RenderingEngine::initializeStageOne()
	{
		DMKRendererSettings settings;
		DMKRenderer::initializeStageOne(DMKRenderingAPI::DMK_RENDERING_API_VULKAN, settings);
	}
}