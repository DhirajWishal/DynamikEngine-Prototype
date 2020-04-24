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

/* -----||||| MAIN LAUNCH SYSTEM |||||----- */

using namespace Dynamik;

#ifdef DMK_PLATFORM_WINDOWS
/* ---------- ########## ////////// MAIN LOOP \\\\\\\\\\ ########## ---------- */
int main(int argc, char** argv) {
	try {
		DMKInstanceDescriptor engineInstance;
		DMKEngine::initializeInstance(engineInstance);

		Camera camera;
		DMKEngine::setupCamera(&camera);

		OceanSkybox skybox;

		DMKSceneDescriptor scene1;
		scene1.sceneID = "S001";
		scene1.sceneIndex = 0;
		scene1.assets = { &skybox };

		DMKLevelDescriptor level1;
		level1.levelID = "001";
		level1.levelIndex = 0;
		level1.scenes = { scene1 };

		DMKEngine::addLevel(level1);

		/* Initialize the renderer */
		DMKEngine::initializeRendererStageOne();
		DMKEngine::genarateRenderables();
		DMKEngine::initializeRendererStageTwo();
		DMKEngine::initializeRendererStageThree();

		DMKEngine::run();
	}
	catch (std::exception& e) {
		std::cout << e.what();

		return DMK_FAIL;
	}

	return DMK_SUCCESS;
}

#endif