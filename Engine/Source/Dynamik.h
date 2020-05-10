#pragma once
#ifndef _DYNAMIK_H
#define _DYNAMIK_H

/*
 Dynamik Engine main header.

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		15/06/2019	(DEVELOPMENTS ONGOING)
 IDE:		MS Visual Studio Community 2019

 This file contains the main DMKEngine class which is the Dynamik Engine API.

 Dynamik.h file.
*/

/* CORE INCLUDES */
#include "Core/Algorithm/AlgorithmLib.h"
#include "Core/DataTypes/DataTypesLib.h"
#include "Core/Memory/MemoryLib.h"

/* GAME OBJECTS */
#include "GameObject.h"
#include "SkyboxObject.h"
#include "KeyBingings.h"

#include "Managers/Managers.h"
#include "Renderer/Renderer.h"
#include "Platform/Windows/Window/WindowsWindow.h"

#include "FileSystem/FileManager.h"

#include "Camera.h"

/* Managers */
#include "Managers/Managers.h"
#include "Events/EventManager.h"

/* Rendering engine */
#include "Renderer/Renderer.h"

/* Audio engine */
#include "Audio/Audio.h"

namespace Dynamik {
	using namespace Renderer;

	enum class DMKInstanceMode {
		DMK_INSTANCE_MODE_DEBUG,
		DMK_INSTANCE_MODE_RELEASE,
		DMK_INSTANCE_MODE_DISTRIBUTE,
	};

	enum class DMKEngineValidationMode {
		DMK_ENGINE_VALIDATION_MODE_NONE,
		DMK_ENGINE_VALIDATION_MODE_STD_COUT,
		DMK_ENGINE_VALIDATION_MODE_TEXT_FILE,
	};

	struct DMKInstanceDescriptor {
		CCPTR DynamikName = "Dynamik";
		CCPTR windowTitle = "Dynamik Engine";

		DMKRenderingAPI renderingAPI = DMKRenderingAPI::DMK_RENDERING_API_VULKAN;
		CCPTR iconPath = "E:/Projects/Dynamik Engine/Versions/Dynamik/Dependencies/Assets/icons/Dynamik.jpg";
		DMKRendererSettings settings;

		DMKInstanceMode mode = DMKInstanceMode::DMK_INSTANCE_MODE_DEBUG;
		B1 enableEngineValidation = true;
		DMKEngineValidationMode validationMode = DMKEngineValidationMode::DMK_ENGINE_VALIDATION_MODE_STD_COUT;
		CCPTR engineValidationTextOutputPath = "";

		ARRAY<CCPTR> engineStartupImages;
	};

	/* MAIN DYNAMIK ENGINE API OBJECT
	 * SINGLETON
	 *
	 * This class contains all the necessary commands to initialize, run and terminate the Dynamik Engine.
	 * DMKEngine class contains APIs for:
	 *	- Advanced Dynamik Graphics Renderer (ADGR)
	 *	- Audio Engine
	 *	- Physics Engine
	 *	- Desctruction Engine
	 *
	 * This class handles resouces internally.
	 */
	class DMKEngine {
		DMKEngine();
		~DMKEngine();
		static DMKEngine instance;

	public:
		DMKEngine(const DMKEngine&) = delete;
		DMKEngine(DMKEngine&&) = delete;
		DMKEngine& operator=(const DMKEngine&) = delete;
		DMKEngine& operator=(DMKEngine&&) = delete;

		/* Dynamik commands */
		static void initializeInstance(DMKInstanceDescriptor descriptor);
		static UI32 addLevel(DMKLevelDescriptor level);
		static void loadLevel(UI32 levelIndex);
		static void loadScene(UI32 sceneIndex);
		static void loadCurrentScene();
		static UI32 addAsset(DMKGameObject* object);

		static void updateSceneIndex(UI32 index);
		static void updateLevelIndex(UI32 index);

		static void updateMovementSpeed(F32 speed);
		static void updateMouseSensitivity(F32 sensitivity);
		static void updateAnimationSpeed(F32 speed);

		static void setupCamera(DMKCamera* camera);

		static void terminateInstance();

		/* Rendering engine commands */
		static void initializeRendererStageOne();
		static void genarateRenderables();
		static void submitLoadedAssets();
		static void addToRenderingQueue(DMKGameObject* object);
		static void initializeRendererStageTwo();
		static void initializeRendererStageThree();

		/* Main loop */
		static void run();

		/* Termination */
		static void terminate();

	private:
		inline static B1 isRenderableAsset(AssetContainer asset);
		inline static void cleanUniformBuffers();
		inline static void onUpdateCleanup();

		AssetManager myAssetManager = {};
		UI32 levelIndex = 0;
		UI32 sceneIndex = 0;
		UI32 progress = 0;

		DMKInstanceDescriptor myInstanceDescriptor = {};

		DMKCamera* myCamera = nullptr;
		DMKCameraData cameraData = {};
		F32 FOV = 30.0f;
		F32 aspectRatio = 0.5f;
		F32 frustumNear = 0.001f;
		F32 frustumFar = 256.0f;

		ARRAY<POINTER<DMKEventComponent>> eventComponents;

		ARRAY<POINTER<InternalFormat>> internalFormats;

	private:
		F32 movementSpeed = 1.0f;
		F32 mouseSensitivity = 1.0f;
		F32 animationSpeed = 1.0f;

	private:
		UI32 maximumUsableThreadCount = 0;
		std::thread myStartypRendererThread;
	};

	/*
	 Contains utility functions for the client application.
	*/
	class DMKUtilities {
	public:
		DMKUtilities() = delete;
		~DMKUtilities() = delete;
		DMKUtilities(const DMKUtilities&) = delete;
		DMKUtilities(DMKUtilities&&) = delete;
		DMKUtilities& operator=(const DMKUtilities&) = delete;
		DMKUtilities& operator=(DMKUtilities&&) = delete;

		/* Validates and returns the key event component. Returns an empty component if the given component is not valid. */
		static DMKKeyEventComponent getKeyEvent(POINTER<DMKEventComponent> component);
		/* Validates and returns the mouse button event component. Returns an empty component if the given component is not valid. */
		static DMKMouseButtonEventComponent getMouseButtonEvent(POINTER<DMKEventComponent> component);
		/* Validates and returns the mouse scroll event component. Returns an empty component if the given component is not valid. */
		static DMKMouseScrollEventComponent getMouseScrollEvent(POINTER<DMKEventComponent> component);
	};
}

#endif // !_DYNAMIK_H
