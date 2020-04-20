#pragma once
#ifndef _DYNAMIK_APPLICATION_H
#define _DYNAMIK_APPLICATION_H

#include "core.h"
#include "GameObject.h"

#include "Managers/Managers.h"

namespace Dynamik {
	enum class DMKRenderingAPI {
		DMK_RENDERING_API_VULKAN,
		DMK_RENDERING_API_OPENGL,
		DMK_RENDERING_API_DIRECTX_12,
	};

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
		CCPTR applicationName = "Dynamik";
		CCPTR windowTitle = "Dynamik Engine";

		DMKRenderingAPI renderingAPI = DMKRenderingAPI::DMK_RENDERING_API_VULKAN;
		CCPTR iconPath = "";

		DMKInstanceMode mode = DMKInstanceMode::DMK_INSTANCE_MODE_DEBUG;
		B1 enableEngineValidation = true;
		DMKEngineValidationMode validationMode = DMKEngineValidationMode::DMK_ENGINE_VALIDATION_MODE_STD_COUT;
		CCPTR engineValidationTextOutputPath = "";

		ARRAY<CCPTR> engineStartupImages;
	};

	class DMK_API Application {
		Application() {}

	public:
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		static void initializeInstance(DMKInstanceDescriptor descriptor);

		static UI32 addLevel(DMKLevelDescriptor level);
		static void loadLevel(UI32 levelIndex);

		static void loadScene(UI32 sceneIndex);

		static UI32 addAsset(DMKGameObject* object);

		static void initializeRenderingEngine();

	private:
		static Application& instance();

		AssetManager myAssetManager;
		UI32 levelIndex = 0;
		UI32 sceneIndex = 0;
	};
}

#endif // !_DYNAMIK_APPLICATION_H
