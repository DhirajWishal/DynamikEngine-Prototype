#include "engineInstance.h"

Engine::Engine(Dynamik::ARRAY<Dynamik::Scene*>& scene) : Application(scene) {
	//pushLayer(new layer);
}

Engine::~Engine() {
}

Dynamik::Application* Dynamik::createApplication(Dynamik::ARRAY<Dynamik::Scene*>& scene) {
	return new Engine(scene);
}