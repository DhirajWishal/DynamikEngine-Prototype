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
#include "Core/Algorithm/AlgorithmLib.h"
#include "Core/DataTypes/DataTypesLib.h"
#include "Core/Memory/MemoryLib.h"

/* GAME OBJECTS */
#include "GameObject.h"
#include "SkyboxObject.h"
#include "KeyBingings.h"

#include "Managers/Managers.h"
#include "Renderer/Renderer.h"
#include "Platform/Windows/Window/WindowManager.h"

#include "Camera.h"

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

	struct  DMKInstanceDescriptor {
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
}

#endif // !_DYNAMIK_H
