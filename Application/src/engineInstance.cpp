#include "engineInstance.h"

Engine::Engine(std::vector<Dynamik::Scene*>& scene) : Application(scene) {
	//pushLayer(new layer);
}

Engine::~Engine() {
}

Dynamik::Application* Dynamik::createApplication(std::vector<Dynamik::Scene*>& scene) {
	return new Engine(scene);
}