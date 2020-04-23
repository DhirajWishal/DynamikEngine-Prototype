/*
 Main application file.

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019
*/

#include "Dynamik.h"
#include "Core/PCH/dmkafx.h"

#include "src/Skybox.h"

/* -----||||| MAIN LAUNCH SYSTEM |||||----- */

using namespace Dynamik;

#ifdef DMK_PLATFORM_WINDOWS
/* ---------- ########## ////////// MAIN LOOP \\\\\\\\\\ ########## ---------- */
int main(int argc, char** argv) {
	try {
		DMKInstanceDescriptor engineInstance;
		DMKEngine::initializeInstance(engineInstance);

		OceanSkybox skybox;
		DMKEngine::addAsset(&skybox);

		DMKEngine::run();
	}
	catch (std::exception& e) {
		std::cout << e.what();

		return DMK_FAIL;
	}

	return DMK_SUCCESS;
}

#endif