#include "dmkafx.h"
#include "application.h"

namespace Dynamik {

	Application::Application() {
		myRenderingEngine.setMipLevel(1.0f);

		myRenderingEngine.initRenderer();
	}

	Application::~Application() {
		myRenderingEngine.end();
	}

	DMK_API void Application::run() {
		myLoader.run();

		while (!myRenderingEngine.getWindowCloseEvent()) {
			myRenderingEngine.draw();
			//myEvent = &myRenderingEngine.pollEvents();

			onEvent(*myRenderingEngine.pollEvents());

			for (auto layer : layerStack)
				layer->update();
		}

		myRenderingEngine.idleCall();
	}

	void Application::pushLayer(ADGR::Layer* layer) {
		layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(ADGR::Layer* layer) {
		layerStack.pushOverLay(layer);
	}

	void Application::onEvent(Event& event) {
		if (&event != nullptr) {
			EventDispatcher dispatcher(event);

			DMK_CORE_INFO("Im on Event!");

			for (auto it = layerStack.end(); it != layerStack.begin();) {
				(*--it)->onEvent(event);
				if (event.handled)
					break;
			}
		}
	}
}