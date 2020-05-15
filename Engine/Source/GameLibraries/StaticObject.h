#pragma once
#ifndef _DYNAMIK_STATIC_OBJECT_H
#define _DYNAMIK_STATIC_OBJECT_H

#include "GameObject.h"

namespace Dynamik {
	/* Dynamik Static Object
	 * The static object is any object that needs to be rendered.
	 */
	class DMKStaticObject : public DMKGameObject {
	public:
		DMKStaticObject() 
		{
		}
		virtual ~DMKStaticObject() {}

		virtual void initialize() override
		{
			/* Basic description */
			type = DMKObjectType::DMK_OBJECT_TYPE_STATIC;

			/* Asset description */
			descriptor.assetDescription.dynamikResouceFilePath = "E:/Projects/Dynamik Engine/Game Repository/assets/assets/moon";
			descriptor.assetDescription.physicallyBased = false;
			descriptor.assetDescription.textureInputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_IMAGE;
			descriptor.assetDescription.textureType = DMKTextureType::DMK_TEXTURE_TYPE_2D;

			/* Transformation description */
			descriptor.transformDescriptor.location = { 0.0f, 0.0f, 0.0f };

			/* Vertex attributes description */
			DMKVertexAttribute vAttribute1;		/* location = 0 */
			vAttribute1.name = DMKVertexData::DMK_VERTEX_DATA_POSITION;
			vAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
			descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute1);

			DMKVertexAttribute vAttribute2;		/* location = 1 */
			vAttribute2.name = DMKVertexData::DMK_VERTEX_DATA_COLOR;
			vAttribute2.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
			descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute2);

			DMKVertexAttribute vAttribute3;		/* location = 2 */
			vAttribute3.name = DMKVertexData::DMK_VERTEX_DATA_TEXTURE_COORDINATES;
			vAttribute3.dataType = DMKDataType::DMK_DATA_TYPE_VEC2;
			descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute3);

			DMKVertexAttribute vAttribute4;		/* location = 3 */
			vAttribute4.name = DMKVertexData::DMK_VERTEX_DATA_INTEGRITY;
			vAttribute4.dataType = DMKDataType::DMK_DATA_TYPE_F32;
			descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute4);

			/* Index buffer description */
			descriptor.indexBufferType = DMKDataType::DMK_DATA_TYPE_UI32;

			/* Uniform attributes description */
			DMKUniformBufferObjectDescriptor UBODescriptor;
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
			TexSampler.type = DMKUniformType::DMK_UNIFORM_TYPE_IMAGE_SAMPLER_2D;
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

			MAT4 model = glm::translate(MAT4(1.0f), descriptor.transformDescriptor.location);
			uniformBufferDataStore.addData(&model, sizeof(MAT4), 0);
			uniformBufferDataStore.addData(&data.viewMatrix, sizeof(MAT4), 1);
			uniformBufferDataStore.addData(&data.projectionMatrix, sizeof(MAT4), 2);

			return uniformBufferDataStore;
		}

		virtual MAT4 getModelMatrix() { return myModelMatrix; }

	protected:
		MAT4 myModelMatrix = MAT4(1.0f);
	};
}

#endif // !_DYNAMIK_STATIC_OBJECT_H
