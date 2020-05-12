/*
 Main application file.

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019
*/

#include "Dynamik.h"
#include "Core/PCH/dmkafx.h"

#include "src/Camera.h"
#include "src/Skybox.h"
#include "src/Moon.h"
#include "src/Reflect.h"

/* -----||||| MAIN LAUNCH SYSTEM |||||----- */

using namespace Dynamik;

#ifdef DMK_PLATFORM_WINDOWS
/* ---------- ########## ////////// MAIN LOOP \\\\\\\\\\ ########## ---------- */
int main(int argc, char** argv) {
	try 
	{
		DMKInstanceDescriptor engineInstance;
		DMKEngine::initializeInstance(engineInstance);

		Camera camera;
		DMKEngine::setupCamera(&camera);

		/* Level initialization */
		OceanSkybox skybox;
		Moon moon;
		moon.descriptor.assetDescription.dynamikResouceFilePath = "E:/Projects/Dynamik Engine/Game Repository/assets/assets/Reflect";
		moon.descriptor.assetDescription.textureType = DMKTextureType::DMK_TEXTURE_TYPE_CUBEMAP;
		moon.descriptor.assetDescription.textureInputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_IMAGE;

		Reflect reflectObject;

		DMKSceneDescriptor scene1;
		scene1.sceneID = "S001";
		scene1.sceneIndex = 0;
		//scene1.assets = { &skybox };
		scene1.assets = { &skybox, &moon };

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
	}

	return 0;
}

#endif