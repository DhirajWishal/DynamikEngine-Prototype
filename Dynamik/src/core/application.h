#pragma once

#include "core.h"
#include "src/Renderer.h"
#include "event.h"
#include "src/Layers/layer.h"
#include "src/Layers/layerStack.h"

namespace Dynamik {
	class DMK_API Application {
	public:
		Application();
		virtual ~Application();

		void run();

		void pushLayer(ADGR::Layer* layer);
		void pushOverlay(ADGR::Layer* layer);

	private:
		ADGR::Renderer myRenderingEngine;

		ADGR::layerStack layerStack;
	};

	// Defined by the Client
	Application* createApplication();

}