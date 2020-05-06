#pragma once
#ifndef _DYNAMIK_GAME_OBJECT_DESCRIPTORS_H
#define _DYNAMIK_GAME_OBJECT_DESCRIPTORS_H

#include "Public/Array.h"
#include "Dynamik/DMKDataType.h"
#include "Dynamik/objectSpecifications.h"
#include "Dynamik/RenderingSpecifications.h"

namespace Dynamik {
	enum class DMKTextureType {
		DMK_TEXTURE_TYPE_2D,
		DMK_TEXTURE_TYPE_3D,
		DMK_TEXTURE_TYPE_CUBEMAP,
	};

	enum class DMKTextureInputType {
		DMK_TEXTURE_INPUT_TYPE_IMAGE,
		DMK_TEXTURE_INPUT_TYPE_KTX,
		DMK_TEXTURE_INPUT_TYPE_HDRI,
		DMK_TEXTURE_INPUT_TYPE_AUTO,
	};

	struct AssetDescriptor {
		CCPTR dynamikResouceFilePath = "";

		F32 integrty = 1.0f;
		B1 physicallyBased = false;
		std::string materialName = "";

		DMKFormat textureFormat = DMKFormat::DMK_FORMAT_RGBA_8_UNIFORM;
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

	struct  DMKCameraData {
		VEC3 cameraPosition = { 0.0f, 0.0f, 0.0f };
		VEC3 cameraUp = { 0.0f, 1.0f, 0.0f };
		VEC3 cameraFront = { 1.0f, 0.0f, 0.0f };
		VEC3 cameraRight = { 1.0f, 0.0f, 0.0f };

		F32 fieldOfView = 60.0f;
		F32 aspectRatio = 0.5f;
		F32 cameraFar = 256.0f;
		F32 cameraNear = 0.001f;
	};

	/* Binding indexes are assigned in the same order as the vertex attributes are submitted. */
	struct  DMKVertexAttribute {
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

	struct  DMKUniformAttribute {
		DMKUniformData name = DMKUniformData::DMK_UNIFORM_DATA_MODEL;
		DMKDataType dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
	};

	class DMKUniformBufferObjectDescriptor {
	public:
		DMKUniformBufferObjectDescriptor() {}
		~DMKUniformBufferObjectDescriptor() {}

		static UI32 uniformByteSize(ARRAY<DMKUniformAttribute> attributes);

		DMKUniformType type = DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT;
		DMKShaderLocation location = DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX;
		ARRAY<DMKUniformAttribute> attributes;
		UI32 binding = 0;
	};

	/* Render Specifications */
	struct  DMKRenderSpecification {
		DMKSwapChainType swapChainType = DMKSwapChainType::DMK_SWAPCHAIN_TYPE_DEFAULT;
		DMKRenderPassType renderPassType = DMKRenderPassType::DMK_RENDER_PASS_TYPE_DEFAULT;
		DMKFrameBufferType frameBufferType = DMKFrameBufferType::DMK_FRAME_BUFFER_TYPE_MAIN;
		DMKPipelineMSAASamples msaaSamples = DMKPipelineMSAASamples::DMK_PIPELINE_MSAA_SAMPLES_1;
		DMKPipelineFrontFace pipelineFrontFace = DMKPipelineFrontFace::DMK_PIPELINE_FRONT_FACE_POSITIVE;
		DMKPipelinePolygonMode pipelinePolygonMode = DMKPipelinePolygonMode::DMK_PIPELINE_POLYGON_MODE_FILL;
		DMKPipelineTopology pipelineTopology = DMKPipelineTopology::DMK_PIPELINE_TOPOLOGY_TRIANGLE_LIST;
		DMKRenderingTechnology renderingTechnology = DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_INDEXED;

		ARRAY<DMKRenderAttachment> renderAttachments;
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

		DMKRenderSpecification renderSpecification;

		DMKVertexBufferObjectDescriptor vertexBufferObjectDescription;
		DMKDataType indexBufferType = DMKDataType::DMK_DATA_TYPE_UI32;
		ARRAY<DMKUniformBufferObjectDescriptor> uniformBufferObjectDescriptions;
	};

	/* Uniform buffer data typedef */
	 typedef ARRAY<MAT4> DMKUniformBufferData;
}

#endif // !_DYNAMIK_GAME_OBJECT_DESCRIPTORS_H