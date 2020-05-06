/*
 Main Dynamik Studio Entry point file.

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		24/02/2020
 IDE:		MS Visual Studio Community 2019
*/

#include "studioafx.h"
#include "Dynamik.h"

#include <filesystem>

#include "Platform/Windows.h"
#include "Core/Objects/Camera.h"
#include "Core/Objects/Moon.h"
#include "Core/Objects/Skybox.h"

#include "Core/Engine/RenderingEngine.h"
using namespace Studio;

#define _SOLUTION_DIR

int main(int argc, char* argv[]) {
	try
	{
		DMKInstanceDescriptor engineInstance;
		DMKEngine::initializeInstance(engineInstance);

		Camera camera;
		DMKEngine::setupCamera(&camera);

		/* Level initialization */
		OceanSkybox skybox;
		//skybox.descriptor.renderSpecification.renderingTechnology = DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_VERTEX;
		Moon moon;
		//moon.descriptor.renderSpecification.renderingTechnology = DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_VERTEX;
		//moon.descriptor.transformDescriptor.location = { 0.0f, 0.0f, 1.0f };

		DMKSceneDescriptor scene1;
		scene1.sceneID = "S001";
		scene1.sceneIndex = 0;
		scene1.assets = { &skybox, &moon };
		//scene1.assets = { &moon };

		DMKLevelDescriptor level1;
		level1.levelID = "001";
		level1.levelIndex = 0;
		level1.scenes = { scene1 };

		DMKEngine::addLevel(level1);

		/* Initialize the renderer */
		DMKEngine::loadCurrentScene();
		DMKEngine::initializeRendererStageOne();
		DMKEngine::genarateRenderables();
		DMKEngine::submitLoadedAssets();
		DMKEngine::initializeRendererStageTwo();
		DMKEngine::initializeRendererStageThree();

		DMKEngine::run();

		DMKEngine::terminate();
	}
	catch (std::exception& e)
	{
		std::cout << e.what();

		return -1;
	}

	return 0;

}