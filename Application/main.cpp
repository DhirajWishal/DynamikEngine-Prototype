/*
 Main application file.

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019
*/

#include "src/engineInstance.h"
#include "src/imports.h"

/* -----||||| MAIN LAUNCH SYSTEM |||||----- */

#ifdef DMK_PLATFORM_WINDOWS
/* ---------- ########## ////////// MAIN LOOP \\\\\\\\\\ ########## ---------- */
int main(int argc, char** argv) {
	try {
		auto paths = getTexturePath(readFile("E:/Projects/Dynamik Engine/Dynamik/Application/paths.txt"));

		std::vector<Dynamik::GameObject> gameObjects = {};

		std::vector<std::vector<float>> locations = {
			{0.0f,	0.0f,	0.0f},
			{0.0f,	5.0f,	0.0f},
			{0.0f,	-5.0f,	0.0f},
			{0.0f,	10.0f,	0.0f},
			{0.0f,	-10.0f,	0.0f},

			{5.0f,	0.0f,	0.0f},
			{5.0f,	5.0f,	0.0f},
			{5.0f,	-5.0f,	0.0f},
			{5.0f,	10.0f,	0.0f},
			{5.0f,	-10.0f,	0.0f},

			{-5.0f,	0.0f,	0.0f},
			{-5.0f,	5.0f,	0.0f},
			{-5.0f,	-5.0f,	0.0f},
			{-5.0f,	10.0f,	0.0f},
			{-5.0f,	-10.0f,	0.0f},

			{10.0f,	0.0f,	0.0f},
			{10.0f,	5.0f,	0.0f},
			{10.0f,	-5.0f,	0.0f},
			{10.0f,	10.0f,	0.0f},
			{10.0f,	-10.0f,	0.0f},

			{-10.0f,	0.0f,	0.0f},
			{-10.0f,	5.0f,	0.0f},
			{-10.0f,	-5.0f,	0.0f},
			{-10.0f,	10.0f,	0.0f},
			{-10.0f,	-10.0f,	0.0f},
		};

		std::vector<Dynamik::DMKObjectType> objTypes = {
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT,
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT,
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT,
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_AI,
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT,
		};

		for (int i = 0; i < paths.size(); i++) {
			Dynamik::GameObjectProperties props;
			props.name = "Charlet";
			props.ID = std::to_string(i);
			props.location = paths[i];
			props.transformProperties.location = locations[i];

			gameObjects.push_back(charlet(props));
		}

		auto application = Dynamik::createApplication(gameObjects);
		application->run();

		delete application;
	}
	catch (std::exception & e) {
		std::cout << e.what();

		return DMK_FAIL;
	}

	return DMK_SUCCESS;
}

#endif