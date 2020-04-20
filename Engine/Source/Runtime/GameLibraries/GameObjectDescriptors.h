#pragma once
#ifndef _DYNAMIK_GAME_OBJECT_DESCRIPTORS_H
#define _DYNAMIK_GAME_OBJECT_DESCRIPTORS_H

#include "Public/datatypes.h"

namespace Dynamik {
	struct AssetDescriptor {
		CCPTR dynamikResouceFilePath = "";

		F32 integrty = 1.0f;
	};

	struct TransformDescriptor {
		VEC3 location = { 0.0f, 0.0f, 0.0f };
		VEC3 rotation = { 0.0f, 0.0f, 0.0f };

		F32 movementBias = 2.5f;
	};

	struct CameraDescriptor {
		B1 modelLock = false;
		B1 viewLock = false;
	};

	struct AudioDescriptor {
		CCPTR assetPath = "";
		F32 volume = 0.0f;
		//F32 
	};

	enum class DMKGameObjectLightingType {
		DMK_GAME_OBJECT_LIGHTING_TYPE_EMISSIVE,
		DMK_GAME_OBJECT_LIGHTING_TYPE_DIFFUSE,
		DMK_GAME_OBJECT_LIGHTING_TYPE_REFLECTIVE,
	};

	struct LightingDescriptor {
		VEC3 direction = { 0.0f, 0.0f, 0.0f };
		DMKGameObjectLightingType type = DMKGameObjectLightingType::DMK_GAME_OBJECT_LIGHTING_TYPE_REFLECTIVE;
		F32 reductionFactor = 1.0f;
	};

	class DMKGameObjectDescriptor {
	public:
		DMKGameObjectDescriptor() {}
		~DMKGameObjectDescriptor() {}

		AssetDescriptor assetDescription;
		TransformDescriptor transformDescriptor;
		CameraDescriptor cameraDescriptor;
		AudioDescriptor audioDescriptor;
		LightingDescriptor lightingDescriptor;
	};
}

#endif // !_DYNAMIK_GAME_OBJECT_DESCRIPTORS_H
