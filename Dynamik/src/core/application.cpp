#include "dmkafx.h"
#include "application.h"

namespace Dynamik {

	Application::Application() {
		myRenderingEngine.setMipLevel(5.0f);

		myRenderingEngine.initRenderer();
	}

	Application::~Application() {
		myRenderingEngine.end();
	}

	DMK_API void Application::run() {
		while (true) {
			myRenderingEngine.run();
			for (auto layer : layerStack)
				layer->update();
		}
	}

	void Application::pushLayer(ADGR::Layer* layer) {
		//layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(ADGR::Layer* layer) {
		//layerStack.pushOverLay(layer);
	}

	void Application::onEvent(Event& event) {
		//EventDispatcher dispatcher(event);
		//
		//for (auto it = layerStack.end(); it != layerStack.begin();) {
		//	(*--it)->onEvent(event);
		//	if (event.handled)
		//		break;
		//}
	}
}