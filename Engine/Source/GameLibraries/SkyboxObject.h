#pragma once
#ifndef _DYNAMIK_SKYBOX_OBJECT_H
#define _DYNAMIK_SKYBOX_OBJECT_H

#include "GameObject.h"

namespace Dynamik {
	/* Dynamik SkyBox Object
	 * This contains the basic base features of a Dynamik Skybox object.
	 */
	class DMKSkyboxObject : public DMKGameObject {
	public:
		DMKSkyboxObject()
		{
		}
		virtual ~DMKSkyboxObject() {}

		virtual void initialize() override
		{
			/* Basic description */
			type = DMKObjectType::DMK_OBJECT_TYPE_SKYBOX;

			/* Asset description */
			descriptor.assetDescription.dynamikResouceFilePath = "E:/Projects/Dynamik Engine/Game Repository/assets/assets/Skybox";
			descriptor.assetDescription.physicallyBased = false;
			descriptor.assetDescription.textureInputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_IMAGE;
			descriptor.assetDescription.textureType = DMKTextureType::DMK_TEXTURE_TYPE_CUBEMAP;

			/* Transformation description */
			descriptor.transformDescriptor.location = { 0.0f, 0.0f, 0.0f };

			/* Vertex attributes description */
			DMKVertexAttribute vAttribute1;		/* location = 0 */
			vAttribute1.name = DMKVertexData::DMK_VERTEX_DATA_POSITION;
			vAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
			descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute1);

			DMKVertexAttribute vAttribute2;		/* location = 1 */
			vAttribute2.name = DMKVertexData::DMK_VERTEX_DATA_TEXTURE_COORDINATES;
			vAttribute2.dataType = DMKDataType::DMK_DATA_TYPE_VEC2;
			descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute2);

			/* Index buffer description */
			descriptor.indexBufferType = DMKDataType::DMK_DATA_TYPE_UI32;

			/* Uniform attributes description */
			DMKUniformBufferObjectDescriptor UBODescriptor;		/* binding = 0 */
			UBODescriptor.type = DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT;
			UBODescriptor.location = DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX;
			UBODescriptor.binding = 0;

			DMKUniformAttribute uAttribute1;
			uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_MODEL;
			uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
			UBODescriptor.attributes.push_back(uAttribute1);

			DMKUniformAttribute uAttribute2;
			uAttribute2.name = DMKUniformData::DMK_UNIFORM_DATA_VIEW;
			uAttribute2.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
			UBODescriptor.attributes.push_back(uAttribute2);

			DMKUniformAttribute uAttribute3;
			uAttribute3.name = DMKUniformData::DMK_UNIFORM_DATA_PROJECTION;
			uAttribute3.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
			UBODescriptor.attributes.push_back(uAttribute3);

			/* Initialize the Uniform Buffer Data Store */
			uniformBufferDataStore.initialize(UBODescriptor.attributes);

			/* Texture sampler */
			DMKUniformBufferObjectDescriptor TexSampler;
			TexSampler.type = DMKUniformType::DMK_UNIFORM_TYPE_IMAGE_SAMPLER_CUBEMAP;
			TexSampler.location = DMKShaderLocation::DMK_SHADER_LOCATION_FRAGMENT;
			TexSampler.binding = 1;

			/* Add the uniform descriptior */
			descriptor.uniformBufferObjectDescriptions.push_back(UBODescriptor);
			descriptor.uniformBufferObjectDescriptions.push_back(TexSampler);
		}

		/* Overridable virtual function */
		virtual DMKUniformBufferData onUpdate(DMKCameraData data) override
		{
			uniformBufferDataStore.clear();

			MAT4 model = glm::mat4(1.0f);
			uniformBufferDataStore.addData(&model, sizeof(MAT4), 0);
			
			MAT4 view = glm::mat4(glm::mat3(glm::lookAt(data.cameraPosition, data.cameraPosition + data.cameraFront, data.cameraUp)));
			uniformBufferDataStore.addData(&view, sizeof(MAT4), 1);
			
			MAT4 projection = glm::perspective(glm::radians(data.fieldOfView), data.aspectRatio, data.cameraNear, data.cameraFar);
			uniformBufferDataStore.addData(&projection, sizeof(MAT4), 2);

			return uniformBufferDataStore;
		}
	};
}

#endif // !_DYNAMIK_SKYBOX_OBJECT_H
