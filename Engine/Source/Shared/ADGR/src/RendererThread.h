#pragma once
#ifndef _DYNAMIK_ADGR_RENDERER_THREAD_H
#define _DYNAMIK_ADGR_RENDERER_THREAD_H

#include "Managers.h"
#include "Renderer.h"

namespace Dynamik {
	struct DMK_API RendererThreadDataContianer : public ThreadDataContainer {

	};

	class DMK_API RendererThread : public Thread {
	public:
		RendererThread() {}
		~RendererThread() {}

		void init() override;
		void mainLoop() override;
		void shutdown() override;

		void addProgress(UI32* progress);
		void addInternalFormats(std::vector<InternalFormat*> formats);

		ADGR::Renderer myRenderer;
	};
}

#endif // !_DYNAMIK_ADGR_RENDERER_THREAD_H
