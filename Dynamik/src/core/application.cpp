#include "dmkafx.h"
#include "application.h"

namespace Dynamik {
	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::run() {
		while (true) {
			myRenderingEngine.initRenderer();
			myRenderingEngine.run();
			myRenderingEngine.end();
		}
	}
}