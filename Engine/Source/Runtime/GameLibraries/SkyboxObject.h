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
			descriptor.vertexBufferObjectDescription.attributes.pushBack(vAttribute1);

			DMKVertexAttribute vAttribute2;		/* location = 1 */
			vAttribute2.name = DMKVertexData::DMK_VERTEX_DATA_TEXTURE_COORDINATES;
			vAttribute2.dataType = DMKDataType::DMK_DATA_TYPE_VEC2;
			descriptor.vertexBufferObjectDescription.attributes.pushBack(vAttribute2);

			/* Index buffer description */
			descriptor.indexBufferType = DMKDataType::DMK_DATA_TYPE_UI32;

			/* Uniform attributes description */
			DMKUniformBufferObjectDescriptor UBODescriptor;		/* binding = 0 */
			UBODescriptor.type = DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT;
			UBODescriptor.location = DMKAttributeLocation::DMK_ATTRIBUTE_LOCATION_VERTEX;
			UBODescriptor.binding = 0;

			DMKUniformAttribute uAttribute1;
			uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_MODEL;
			uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
			UBODescriptor.attributes.pushBack(uAttribute1);

			DMKUniformAttribute uAttribute2;
			uAttribute2.name = DMKUniformData::DMK_UNIFORM_DATA_VIEW;
			uAttribute2.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
			UBODescriptor.attributes.pushBack(uAttribute2);

			DMKUniformAttribute uAttribute3;
			uAttribute3.name = DMKUniformData::DMK_UNIFORM_DATA_PROJECTION;
			uAttribute3.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
			UBODescriptor.attributes.pushBack(uAttribute3);

			/* Texture sampler */
			DMKUniformBufferObjectDescriptor TexSampler;
			TexSampler.type = DMKUniformType::DMK_UNIFORM_TYPE_IMAGE_SAMPLER_CUBEMAP;
			TexSampler.location = DMKAttributeLocation::DMK_ATTRIBUTE_LOCATION_FRAGMENT;
			TexSampler.binding = 1;

			/* Add the uniform descriptior */
			descriptor.uniformBufferObjectDescriptions.pushBack(UBODescriptor);
			descriptor.uniformBufferObjectDescriptions.pushBack(TexSampler);
		}
		virtual ~DMKSkyboxObject() {}

		/* Overridable virtual function */
		virtual DMKUniformBufferData onUpdate(DMKCameraData data) override
		{
			MAT4 model = glm::mat4(1.0f);
			MAT4 view = glm::lookAt(data.cameraPosition, data.cameraPosition + data.cameraRight, data.cameraUp);
			MAT4 projection = glm::perspective(glm::radians(data.fieldOfView), data.aspectRatio, data.cameraNear, data.cameraFar);

			return { model, view, projection };
		}
	};
}

#endif // !_DYNAMIK_SKYBOX_OBJECT_H
