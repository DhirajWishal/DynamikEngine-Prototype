#include "engineInstance.h"

Engine::Engine(std::vector<Dynamik::GameObject>& gameObjects) : Application(gameObjects) {
	//pushLayer(new layer);
}

Engine::~Engine() {
}

Dynamik::Application* Dynamik::createApplication(std::vector<Dynamik::GameObject>& gameObjects) {
	return new Engine(gameObjects);
}
