#pragma once

#include "dmkafx.h"

#include "core/Engines/ADGR/Layers/layerStack.h"
#include "core/application.h"
#include "CentralDataHub.h"
#include "Utilities.h"
#include "debugger.h"
#include "GameObject.h"

//#include "imports.h"

class Engine : public Dynamik::Application {
public:
	Engine(Dynamik::ARRAY<Dynamik::Scene*>& scene);

	~Engine();
};

Dynamik::Application* Dynamik::createApplication(Dynamik::ARRAY<Dynamik::Scene*>& scene);
