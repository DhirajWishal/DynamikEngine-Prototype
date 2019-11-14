#pragma once

#ifndef _DYNAMIK_APPLICATION_H
#define _DYNAMIK_APPLICATION_H

#include "core.h"
#include "src/Renderer.h"
#include "src/Layers/layer.h"
#include "src/Layers/layerStack.h"

#include "inputHandler.h"

#include "Audio.h"

#include "object mechanics/loadGameObjects.h"

namespace Dynamik {
	class DMK_API Application {
	public:
		Application();
		virtual ~Application();

		void run();

		void pushLayer(ADGR::Layer* layer);
		void pushOverlay(ADGR::Layer* layer);

		void onEvent(int ked);

		static void showProgress();

	private:
		ADGR::Renderer myRenderingEngine;

		//Event& myEvent;

		ADGR::layerStack layerStack;

		InputLoader myLoader;

		//inputHandler myInputHandler{ myEvent };

		Audio::AudioEngine myEngine;
	};

	// Defined by the Client
	Application* createApplication();
}

#endif // !_DYNAMIK_APPLICATION_H
