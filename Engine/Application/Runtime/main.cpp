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

UI32 index = 0;

#ifdef DMK_PLATFORM_WINDOWS
/* ---------- ########## ////////// MAIN LOOP \\\\\\\\\\ ########## ---------- */
int main(int argc, char** argv) {
	try {
		auto paths = getTexturePath(readFile("E:\\Projects\\Dynamik Engine\\Versions\\Dynamik (Prototype)\\Engine\\Application\\Runtime\\paths.txt"));

		Dynamik::ARRAY<Dynamik::ARRAY<float>> locations = {
			{0.0f,	0.0f,	0.0f},
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

		Dynamik::ARRAY<Dynamik::DMKObjectType> objTypes = {
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKYBOX,
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT,
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT,
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXT_OVERLAY,
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKYBOX,
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT,
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_AI,
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT,
		};

		Dynamik::ARRAY<mars> marses;
		Dynamik::ARRAY<GameObject*> objects = {};

		for (int i = 0; i < paths.size(); i++) {
			Dynamik::GameObjectProperties props;
			props.name = "Mars";
			props.type = objTypes[i];
			props.ID = std::to_string(i);
			props.location = paths[i];
			props.transformProperties.location = locations[i];
			mars _local;
			_local.myProperties = props;
			marses.pushBack(_local);
		}

		Dynamik::GameObjectProperties _localProps;
		_localProps.type = DMKObjectType::DMK_OBJECT_TYPE_AUDIO;
		_localProps.objectPath = { "E:/Projects/Dynamik Engine/Dynamik/components/Audio/media/explosion.wav" };
		explosion exp(_localProps);
		objects.pushBack(&exp);

		for (int i = 0; i < marses.size(); i++)
			objects.push_back(&marses[i]);

		sceneOne scene(objects);
		Dynamik::ARRAY<Scene*> scenes = { &scene };

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