#pragma once

#include "src/Renderer.h"
#include "core.h"

namespace Dynamik {
	class DMK_API Application {
	public:
		Application();
		virtual ~Application();

		void run();

	private:
		ADGR::Renderer myRenderingEngine;
	};

	// Defined by the Client
	Application* createApplication();

}