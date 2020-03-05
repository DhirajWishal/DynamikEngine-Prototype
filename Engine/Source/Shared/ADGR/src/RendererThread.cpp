#include "adgrafx.h"
#include "RendererThread.h"
#include "Renderer.h"
#include "rendererFormat.h"
#include "rendererCommandQueue.h"

namespace Dynamik {
	void RendererThread::init()
	{
		myRenderer.setRendererFormats(_internalFormats);
		myRenderer.initRenderer();
	}

	void RendererThread::loop()
	{
		myRenderer.getEvents();
		myRenderer.draw();
	}

	void RendererThread::shutdown()
	{
		myRenderer.idleCall();
		myRenderer.end();
	}

	B1 RendererThread::loopEndCommand()
	{
		return myRenderer.getWindowCloseEvent();;
	}

	void RendererThread::addProgress(UI32* progress)
	{
		myRenderer.setProgress(progress);
	}

	void RendererThread::addInternalFormats(std::vector<InternalFormat*> formats)
	{
		_internalFormats = formats;
	}
}