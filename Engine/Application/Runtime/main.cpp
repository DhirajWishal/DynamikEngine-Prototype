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


#ifdef DMK_PLATFORM_WINDOWS
/* ---------- ########## ////////// MAIN LOOP \\\\\\\\\\ ########## ---------- */
int main(int argc, char** argv) {
	try {
		//Dynamik::Application::addAsset();
		OceanSkybox skybox;
		Dynamik::Application::addAsset(&skybox);
	}
	catch (std::exception& e) {
		std::cout << e.what();

		return DMK_FAIL;
	}

	return DMK_SUCCESS;
}

#endif