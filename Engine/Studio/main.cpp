/*
 Main Dynamik Studio Entry point file.

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		24/02/2020
 IDE:		MS Visual Studio Community 2019
*/

#include "studioafx.h"
#include "Dynamik.h"

#include "Platform/Windows.h"
#include "Core/Objects/Camera.h"
#include "Core/Objects/Moon.h"
#include "Core/Objects/Skybox.h"

#include "Core/Engine/RenderingEngine.h"
using namespace Studio;

int main() {

	/* Level initialization */
	OceanSkybox skybox;
	Moon moon;

	DMKSceneDescriptor scene1;
	scene1.sceneID = "S001";
	scene1.sceneIndex = 0;
	scene1.assets = { &skybox, &moon };

	DMKLevelDescriptor level1;
	level1.levelID = "001";
	level1.levelIndex = 0;
	level1.scenes = { scene1 };

	WindowManager myWindowManager;

	DMKWindowManagerInitInfo windowInitInfo;
	windowInitInfo.title = "Dynamik Studio";

	AssetManager myAssetManager;

	try
	{
		myWindowManager.initialize(windowInitInfo);

		RenderingEngine _renderer;
		_renderer.setWindowHandle(myWindowManager.window);
		_renderer.initializeStageOne();
	}
	catch (const std::exception&)
	{

	}
	return 0;
}