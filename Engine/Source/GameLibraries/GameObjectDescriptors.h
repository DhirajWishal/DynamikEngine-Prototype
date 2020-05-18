#pragma once
#ifndef _DYNAMIK_GAME_OBJECT_DESCRIPTORS_H
#define _DYNAMIK_GAME_OBJECT_DESCRIPTORS_H

#include <vector>
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

	enum class DMKAttachmentType {
		DMK_ATTACHMENT_TYPE_CUBE_MAP,
		DMK_ATTACHMENT_TYPE_IMAGE_BASED_LIGHTING,
	};

	struct MaterialDescriptor {
		std::string materialName = "";
		VEC4 baseColor = { 0.0f,0.0f, 0.0f, 1.0f };
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

		F32 hitBoxRadius = 1.0f;

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
		VEC3 rayOrigin = { 0.0f, 0.0f, 0.0f };
		VEC3 rayDirection = { 0.0f, 0.0f, 0.0f };

		MAT4 viewMatrix = glm::mat4(1.0f);
		MAT4 projectionMatrix = glm::mat4(1.0f);
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

		static UI32 vertexByteSize(std::vector<DMKVertexAttribute> attributes);

		std::vector<DMKVertexAttribute> attributes;
	};

	struct  DMKUniformAttribute {
		DMKUniformData name = DMKUniformData::DMK_UNIFORM_DATA_MODEL;
		DMKDataType dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
		UI32 arrayCount = 1;
	};

	class DMKUniformBufferObjectDescriptor {
	public:
		DMKUniformBufferObjectDescriptor() {}
		~DMKUniformBufferObjectDescriptor() {}

		static UI32 uniformByteSize(std::vector<DMKUniformAttribute> attributes);

		DMKUniformType type = DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT;
		DMKShaderLocation location = DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX;
		std::vector<DMKUniformAttribute> attributes;
		UI32 binding = 0;
		UI32 offset = 0;
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

		std::vector<DMKRenderAttachment> renderAttachments;
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
		std::vector<DMKUniformBufferObjectDescriptor> uniformBufferObjectDescriptions;

		std::vector<DMKAttachmentType> additionalAttachments;
	};

	/*
	 Dynamik Uniform Buffer Data Container
	 This container is used to store the uniform buffer data.
	 This is only indended to use for the Uniform Buffer Object.
	*/
	class DMKUniformBufferData {
	public:
		DMKUniformBufferData();
		~DMKUniformBufferData();

		void initialize(std::vector<DMKUniformAttribute> attributes);
		void clear();
		void terminate();
		VPTR data() { return store; }
		UI32 size() { return allocationSize; }

		void addData(VPTR data, UI32 dataSize, UI32 dataLocation);

	private:
		void _incrementPtr(UI32 byteCount);

		VPTR store = nullptr;
		VPTR nextPtr = store;
		B1 isTerminated = false;
		UI32 allocationSize = 0;

		std::vector<DMKUniformAttribute> _attributes;
	};
}

#endif // !_DYNAMIK_GAME_OBJECT_DESCRIPTORS_H
