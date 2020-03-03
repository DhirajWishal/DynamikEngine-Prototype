#include "adgrafx.h"
#include "RendererThread.h"
#include "Renderer.h"
#include "rendererFormat.h"
#include "rendererCommandQueue.h"

namespace Dynamik {
	void RendererThread::init()
	{
		myRenderer.initRenderer();
	}

	void RendererThread::loop()
	{
		myRenderer.draw();
	}

	void RendererThread::shutdown()
	{
		myRenderer.idleCall();
		myRenderer.end();
	}
	
	void RendererThread::addProgress(UI32* progress)
	{
		myRenderer.setProgress(progress);
	}
	
	void RendererThread::addInternalFormats(std::vector<InternalFormat*> formats)
	{
		myRenderer.setRendererFormats(formats);
	}
}