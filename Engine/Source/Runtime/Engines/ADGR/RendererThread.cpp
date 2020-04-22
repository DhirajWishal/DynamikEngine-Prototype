#include "dmkafx.h"
#include "RendererThread.h"
#include "Renderer.h"
#include "rendererFormat.h"
#include "rendererCommandQueue.h"

namespace Dynamik {
	void RendererThread::init()
	{
		//myRenderer.setRenderComponents(_internalFormats);
		myRenderer.initRenderer();
	}

	void RendererThread::loop()
	{
		myRenderer.getEvents();
		myRenderer.drawFrame();
	}

	DMKThreadTreminateReason RendererThread::shutdown()
	{
		myRenderer.idleCall();
		myRenderer.end();

		return DMKThreadTreminateReason::DMK_THREAD_TERMINATION_REASON_NORMAL_TERMINATION;
	}

	B1 RendererThread::loopEndCommand()
	{
		terminate = myRenderer.getWindowCloseEvent();
		return terminate;
	}

	void RendererThread::addProgress(UI32* progress)
	{
		myRenderer.setProgress(progress);
	}

	void RendererThread::addInternalFormats(ARRAY<POINTER<InternalFormat>> formats)
	{
		_internalFormats = formats;
	}
}