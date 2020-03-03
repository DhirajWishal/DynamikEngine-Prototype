/*
 Main application file.

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019
*/

#include "src/engineInstance.h"
#include "src/imports.h"

#include "src/objectDefinitions.h"
#include "Platform.h"
#include "Utilities.h"

/* -----||||| MAIN LAUNCH SYSTEM |||||----- */

#ifdef DMK_PLATFORM_WINDOWS
/* ---------- ########## ////////// MAIN LOOP \\\\\\\\\\ ########## ---------- */
int main(int argc, char** argv) {
	try {
		auto paths = getTexturePath(readFile("E:/Projects/Dynamik Engine/Versions/Dynamik (Prototype)/Engine/Application/Runtime/paths.txt"));

		std::vector<std::vector<float>> locations = {
			{0.0f,	0.0f,	0.0f},
			{-0.5f,	1.5f,	0.0f},
			{1.7f,	-2.0f,	0.0f},
			{2.3f,	7.0f,	0.0f},
			{5.1f,	-6.0f,	0.0f},

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

		std::vector<mars> marses(paths.size());
		std::vector<GameObject*> objects = {};

		for (int i = 0; i < paths.size(); i++) {
			Dynamik::GameObjectProperties props;
			props.name = "Mars";
			props.ID = std::to_string(i);
			props.location = paths[i];
			props.transformProperties.location = locations[i];

			marses[i].myProperties = props;
		}

		Dynamik::GameObjectProperties _localProps;
		_localProps.type = DMKObjectType::DMK_OBJECT_TYPE_AUDIO;
		_localProps.objectPath = { "E:/Projects/Dynamik Engine/Dynamik/components/Audio/media/explosion.wav" };
		explosion exp(_localProps);
		objects.push_back(&exp);

		for (int i = 0; i < marses.size(); i++)
			objects.push_back(&marses[i]);

		sceneOne scene(objects);
		std::vector<Scene*> scenes = { &scene };

		Engine myEngine(scenes);
		myEngine.run();
	}
	catch (std::exception & e) {
		std::cout << e.what();

		return DMK_FAIL;
	}

	return DMK_SUCCESS;
}

#endif