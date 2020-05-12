#pragma once
#ifndef _DYNAMIK_RENDERER_RENDERER_THREAD_H
#define _DYNAMIK_RENDERER_RENDERER_THREAD_H


#include "Renderer.h"

namespace Dynamik {
	struct RendererThreadDataContianer : public ThreadDataContainer {
	};

	/* BASE: Thread
	 * Rendering engine thread.
	 */
	class RendererThread : public Thread {
	public:
		RendererThread() {}
		~RendererThread() {}

		void init() override;
		void loop() override;
		DMKThreadTreminateReason shutdown() override;
		B1 loopEndCommand() override;

		void addProgress(UI32* progress);
		void addInternalFormats(std::vector<POINTER<InternalFormat>> formats);

		std::vector<POINTER<InternalFormat>> _internalFormats;
	};
}

#endif // !_DYNAMIK_RENDERER_RENDERER_THREAD_H
