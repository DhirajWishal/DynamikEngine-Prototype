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

//class layer : public Dynamik::ADGR::Layer {
//public:
//	layer() : Layer("Example") {
//	}
//
//	void update() override {
//		//DMK_INFO("ExampleLayer::update");
//	}
//
//	void onEvent(Dynamik::Event& event) override {
//		DMK_WARN("EVENT");
//	}
//
//private:
//};

class Engine : public Dynamik::Application {
public:
	Engine(std::vector<Dynamik::GameObject>& gameObjects) : Application(gameObjects) {
		//pushLayer(new layer);
	}

	~Engine() {
	}
};

Dynamik::Application* Dynamik::createApplication(std::vector<Dynamik::GameObject>& gameObjects) {
	return new Engine(gameObjects);
}

/* -----||||| MAIN LAUNCH SYSTEM |||||----- */

#ifdef DMK_PLATFORM_WINDOWS

Dynamik::Application* Dynamik::createApplication(std::vector<Dynamik::GameObject>& gameObjects);

/* ---------- ########## ////////// MAIN LOOP \\\\\\\\\\ ########## ---------- */
DYNAMIK_ENGINE_MAIN()

#endif