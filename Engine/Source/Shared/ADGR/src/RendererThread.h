#pragma once
#ifndef _DYNAMIK_ADGR_RENDERER_THREAD_H
#define _DYNAMIK_ADGR_RENDERER_THREAD_H

#include "Managers.h"
#include "Renderer.h"

namespace Dynamik {
	struct DMK_API RendererThreadDataContianer : public ThreadDataContainer {
	};

	/* BASE: Thread
	 * Rendering engine thread.
	 */
	class DMK_API RendererThread : public Thread {
	public:
		RendererThread() {}
		~RendererThread() {}

		void init() override;
		void loop() override;
		ThreadTreminateReason shutdown() override;
		B1 loopEndCommand() override;

		void addProgress(UI32* progress);
		void addInternalFormats(std::vector<InternalFormat*> formats);

		ADGR::Renderer myRenderer;
		std::vector<InternalFormat*> _internalFormats;
	};
}

#endif // !_DYNAMIK_ADGR_RENDERER_THREAD_H
