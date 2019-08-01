#include "dmkafx.h"
#include "application.h"

namespace Dynamik {
	Application::Application() {
		myRenderingEngine.initRenderer();
	}

	Application::~Application() {
		myRenderingEngine.end();
	}

	DMK_API void Application::run() {
		myRenderingEngine.run();
		//while (true) 
	}
}