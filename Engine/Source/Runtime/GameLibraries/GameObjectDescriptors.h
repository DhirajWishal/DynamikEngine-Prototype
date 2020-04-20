#pragma once
#ifndef _DYNAMIK_GAME_OBJECT_DESCRIPTORS_H
#define _DYNAMIK_GAME_OBJECT_DESCRIPTORS_H

#include "Public/Array.h"
#include "Dynamik/DMKDataType.h"

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
		F32 dissipationFactor = 1.0f;
	};

	enum class DMKGameObjectLightingType {
		DMK_GAME_OBJECT_LIGHTING_TYPE_EMISSIVE,
		DMK_GAME_OBJECT_LIGHTING_TYPE_DIFFUSE,
		DMK_GAME_OBJECT_LIGHTING_TYPE_REFLECTIVE,
	};

	struct LightingDescriptor {
		VEC3 direction = { 0.0f, 0.0f, 0.0f };
		DMKGameObjectLightingType type = DMKGameObjectLightingType::DMK_GAME_OBJECT_LIGHTING_TYPE_REFLECTIVE;
		F32 dissipationFactor = 1.0f;
	};

	enum class DMKVertexDataName {
		DMK_VERTEX_DATA_NAME_POSITION,
		DMK_VERTEX_DATA_NAME_COLOR,
		DMK_VERTEX_DATA_NAME_TEXTURE_COORDINATE,
		DMK_VERTEX_DATA_NAME_NORMAL,
		DMK_VERTEX_DATA_NAME_SPACE,
		DMK_VERTEX_DATA_NAME_INTEGRITY,
	};

	struct DMKVertexAttribute {
		DMKVertexDataName name = DMKVertexDataName::DMK_VERTEX_DATA_NAME_POSITION;
		DMKDataType type = DMKDataType::DMK_DATA_TYPE_VEC3;
	};

	class DMKVertexBufferObjectDescriptor {
	public:
		DMKVertexBufferObjectDescriptor() {}
		~DMKVertexBufferObjectDescriptor() {}

		static UI32 vertexByteSize(ARRAY<DMKVertexAttribute> attributes);

		ARRAY<DMKVertexAttribute> attributes;
	};

	enum class DMKUniformDataName {
		DMK_UNIFORM_DATA_NAME_MODEL,
		DMK_UNIFORM_DATA_NAME_VIEW,
		DMK_UNIFORM_DATA_NAME_PROJECTION,
	};

	struct DMKUniformAttribute {
		DMKUniformDataName name = DMKUniformDataName::DMK_UNIFORM_DATA_NAME_MODEL;
		DMKDataType type = DMKDataType::DMK_DATA_TYPE_MAT4;
	};

	class DMKUniformBufferObjectDescriptor {
	public:
		DMKUniformBufferObjectDescriptor() {}
		~DMKUniformBufferObjectDescriptor() {}

		static UI32 uniformByteSize(ARRAY<DMKUniformAttribute> attributes);

		ARRAY<DMKUniformAttribute> attributes;
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

		DMKVertexBufferObjectDescriptor vertexBufferObjectDescription;
		DMKDataType indexBufferType = DMKDataType::DMK_DATA_TYPE_UI32;
		DMKUniformBufferObjectDescriptor uniformBufferObjectDescription;
	};
}

#endif // !_DYNAMIK_GAME_OBJECT_DESCRIPTORS_H
