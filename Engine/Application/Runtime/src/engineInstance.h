#pragma once

#include "dmkafx.h"

#include "Layers/layerStack.h"
#include "core/application.h"
#include "CentralDataHub.h"
#include "Utilities.h"
#include "debugger.h"
#include "GameObject.h"

//#include "imports.h"

class Engine : public Dynamik::Application {
public:
	Engine(std::vector<Dynamik::Scene*>& scene);

	~Engine();
};

Dynamik::Application* Dynamik::createApplication(std::vector<Dynamik::Scene*>& scene);
