#include "adgrafx.h"

#include "src/Renderer.h"

int main() {
	Dynamik::ADGR::Renderer myRenderer;
	myRenderer.initRenderer();
	myRenderer.run();
	myRenderer.end();

	return 0;
}

// topping off at 3,696 code lines (03/08/2019)
