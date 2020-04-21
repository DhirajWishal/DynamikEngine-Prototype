#pragma once
#ifndef _DYNAMIK_H
#define _DYNAMIK_H

/*
 Dynamik Engine main header.

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		15/6/2019	(DEVELOPMENTS ONGOING)
 IDE:		MS Visual Studio Community 2019

 This file contains the main DMKEngine class which is the Dynamik Engine API.

 Dynamik.h file.
*/

/* CORE INCLUDES */
#include "Dynamik.h"

#include "GameObject.h"
#include "SkyboxObject.h"
#include "KeyBingings.h"

#include "core.h"
#include "GameObject.h"

#include "Managers/Managers.h"
#include "Engines/ADGR/Renderer.h"
#include "Platform/Windows/Window/WindowManager.h"

#include "Camera.h"

namespace Dynamik {
	using namespace ADGR;

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
		CCPTR iconPath = "";
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
	class DMK_API DMKEngine {
		DMKEngine() {}
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
		static UI32 addAsset(DMKGameObject* object);

		static void updateSceneIndex(UI32 index);
		static void updateLevelIndex(UI32 index);

		static void updateMovementSpeed(F32 speed);
		static void updateMouseSensitivity(F32 sensitivity);
		static void updateAnimationSpeed(F32 speed);

		static void setupCamera(Camera* camera);

		static void terminateInstance();

		/* Rendering engine commands */
		static void initializeRendererStageOne();
		static void genarateRenderables();
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

		AssetManager myAssetManager;
		UI32 levelIndex = 0;
		UI32 sceneIndex = 0;

		WindowManager myWindowManager;
		UI32 progress = 0;

		DMKInstanceDescriptor myInstanceDescriptor;

		Camera* myCamera;
		DMKCameraData cameraData;
		UI32 FOV = 60.0f;
		UI32 aspectRatio = 0.5f;
		UI32 frustumNear = 0.001f;
		UI32 frustumFar = 256.0f;

		ARRAY<DMKEventComponent*> eventComponents;

		ARRAY<POINTER<InternalFormat>> internalFormats;

	private:
		F32 movementSpeed = 1.0f;
		F32 mouseSensitivity = 1.0f;
		F32 animationSpeed = 1.0f;
	};
}

#endif // !_DYNAMIK_H
