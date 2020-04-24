#pragma once
#ifndef _DYNAMIK_GAME_OBJECT_DESCRIPTORS_H
#define _DYNAMIK_GAME_OBJECT_DESCRIPTORS_H

#include "Public/Array.h"
#include "Dynamik/DMKDataType.h"
#include "Dynamik/objectSpecifications.h"

namespace Dynamik {
	enum class DMKTextureType {
		DMK_TEXTURE_TYPE_2D,
		DMK_TEXTURE_TYPE_3D,
		DMK_TEXTURE_TYPE_CUBEMAP,
	};

	enum class DMKTextureInputType {
		DMK_TEXTURE_INPUT_TYPE_IMAGES,
		DMK_TEXTURE_INPUT_TYPE_KTX,
		DMK_TEXTURE_INPUT_TYPE_HDRI,
		DMK_TEXTURE_INPUT_TYPE_AUTO,
	};

	struct AssetDescriptor {
		CCPTR dynamikResouceFilePath = "";

		F32 integrty = 1.0f;
		B1 physicallyBased = false;
		std::string materialName = "";

		DMKTextureType textureType = DMKTextureType::DMK_TEXTURE_TYPE_2D;
		DMKTextureInputType textureInputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_AUTO;
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

	/* Binding indexes are assigned in the same order as the vertex attributes are submitted. */
	struct DMKVertexAttribute {
		DMKVertexData name = DMKVertexData::DMK_VERTEX_DATA_POSITION;
		DMKDataType dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
	};

	class DMKVertexBufferObjectDescriptor {
	public:
		DMKVertexBufferObjectDescriptor() {}
		~DMKVertexBufferObjectDescriptor() {}

		static UI32 vertexByteSize(ARRAY<DMKVertexAttribute> attributes);

		ARRAY<DMKVertexAttribute> attributes;
	};

	struct DMKUniformAttribute {
		DMKUniformData name = DMKUniformData::DMK_UNIFORM_DATA_MODEL;
		DMKDataType dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
	};

	class DMKUniformBufferObjectDescriptor {
	public:
		DMKUniformBufferObjectDescriptor() {}
		~DMKUniformBufferObjectDescriptor() {}

		static UI32 uniformByteSize(ARRAY<DMKUniformAttribute> attributes);

		DMKUniformType type = DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT;
		DMKAttributeLocation location = DMKAttributeLocation::DMK_ATTRIBUTE_LOCATION_VERTEX;
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
		ARRAY<DMKUniformBufferObjectDescriptor> uniformBufferObjectDescriptions;
	};
}

#endif // !_DYNAMIK_GAME_OBJECT_DESCRIPTORS_H
