#include "smpafx.h"

#include "src/Renderer/Renderer.h"

int main() {
	Dynamik::ADGR::Renderer myRenderer;

	try {
		myRenderer.init();

		while (!myRenderer.closeEvent()) {
			myRenderer.drawFrame();
			myRenderer.events();
		}

		myRenderer.shutdown();
	}
	catch (std::exception & e) {
		std::cout << e.what() << std::endl;

		return -1;
	}

	return 0;
}

// topping off at 3,696 code lines (03/08/2019)