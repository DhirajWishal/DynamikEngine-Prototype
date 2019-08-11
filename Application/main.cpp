/*
 Main application file.

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019
*/

#include <Dynamik.h>
#include "src/imports.h"

class layer : public Dynamik::ADGR::Layer {
public:
	layer() : Layer("Example") {
	}

	void update() override {
		DMK_INFO("ExampleLayer::update");
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
