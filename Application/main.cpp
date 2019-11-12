/*
 Main application file.

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019
*/

#include "dmkafx.h"

#include "Layers/layerStack.h"
#include "core/application.h"
#include "CentralDataHub.h"
#include "core/utils/DMK_Functions.h"

#include "debugger.h"

#include "src/imports.h"

class layer : public Dynamik::ADGR::Layer {
public:
	layer() : Layer("Example") {
	}

	void update() override {
		//DMK_INFO("ExampleLayer::update");
	}

	void onEvent(Dynamik::Event& event) override {
		DMK_WARN("EVENT");
	}

private:
};

class Engine : public Dynamik::Application {
public:
	Engine() {
		pushLayer(new layer);
	}

	~Engine() {
	}
};

Dynamik::Application* Dynamik::createApplication() {
	return new Engine();
}

/* -----||||| MAIN LAUNCH SYSTEM |||||----- */

#ifdef DMK_PLATFORM_WINDOWS

Dynamik::Application* Dynamik::createApplication();

/* ---------- ########## ////////// MAIN LOOP \\\\\\\\\\ ########## ---------- */
int main(int argc, char** argv) {
	try {
		printf("\t\t----------|| Welcome to the Dynamik Engine! ||----------\n");

		DMK_CORE_INFO(12);

		auto application = Dynamik::createApplication();
		application->run();

		delete application;
	}
	catch (std::exception & e) {
		std::cout << e.what();

		return DMK_FAIL;
	}

	return DMK_SUCCESS;
}

#endif