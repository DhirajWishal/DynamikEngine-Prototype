#pragma once
#ifndef _DYNAMIK_ADGR_RENDERER_THREAD_H
#define _DYNAMIK_ADGR_RENDERER_THREAD_H

#include "Managers.h"

namespace Dynamik {
	struct RendererThreadDataContianer : public ThreadDataContainer {

	};

	class RendererThread : public Thread {
	public:
		RendererThread() {}
		~RendererThread() {}

		void init() override;
		void mainLoop() override;
		void shutdown() override;
	};
}

#endif // !_DYNAMIK_ADGR_RENDERER_THREAD_H
