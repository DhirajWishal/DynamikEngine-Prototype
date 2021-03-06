#include "Reflect.h"
using namespace Dynamik;

Reflect::Reflect()
{
}

void Reflect::initialize()
{
	/* Basic description */
	type = DMKObjectType::DMK_OBJECT_TYPE_STATIC;

	/* Asset description */
	descriptor.assetDescription.dynamikResouceFilePath = "E:/Projects/Dynamik Engine/Game Repository/assets/assets/Reflect";
	descriptor.assetDescription.textureType = DMKTextureType::DMK_TEXTURE_TYPE_CUBEMAP;
	descriptor.assetDescription.textureInputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_IMAGE;
	descriptor.assetDescription.physicallyBased = false;

	/* Transformation description */
	descriptor.transformDescriptor.location = { 0.0f, 0.0f, 0.0f };

	/* Vertex attributes description */
	DMKVertexAttribute vAttribute1;		/* location = 0 */
	vAttribute1.name = DMKVertexData::DMK_VERTEX_DATA_POSITION;
	vAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
	descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute1);

	DMKVertexAttribute vAttribute2;		/* location = 1 */
	vAttribute2.name = DMKVertexData::DMK_VERTEX_DATA_UV_COORDINATES;
	vAttribute2.dataType = DMKDataType::DMK_DATA_TYPE_VEC2;
	descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute2);

	DMKVertexAttribute vAttribute3;		/* location = 2 */
	vAttribute3.name = DMKVertexData::DMK_VERTEX_DATA_NORMAL_VECTORS;
	vAttribute3.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
	descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute3);

	//DMKVertexAttribute vAttribute4;		/* location = 3 */
	//vAttribute4.name = DMKVertexData::DMK_VERTEX_DATA_NORMAL_VECTORS;
	//vAttribute4.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
	//descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute4);

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

	DMKUniformAttribute uAttribute4;
	uAttribute4.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute4.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
	UBODescriptor.attributes.push_back(uAttribute4);

	/* Initialize the Uniform Buffer Data Store */
	uniformBufferDataStore.initialize(UBODescriptor.attributes);

	/* Texture sampler */
	DMKUniformBufferObjectDescriptor TexSampler;
	TexSampler.type = DMKUniformType::DMK_UNIFORM_TYPE_IMAGE_SAMPLER_CUBEMAP;
	TexSampler.location = DMKShaderLocation::DMK_SHADER_LOCATION_FRAGMENT;
	TexSampler.binding = 2;
	descriptor.uniformBufferObjectDescriptions.push_back(TexSampler);

	TexSampler.type = DMKUniformType::DMK_UNIFORM_TYPE_IMAGE_SAMPLER_2D;
	TexSampler.location = DMKShaderLocation::DMK_SHADER_LOCATION_FRAGMENT;
	TexSampler.binding = 3;
	descriptor.uniformBufferObjectDescriptions.push_back(TexSampler);

	TexSampler.type = DMKUniformType::DMK_UNIFORM_TYPE_IMAGE_SAMPLER_CUBEMAP;
	TexSampler.location = DMKShaderLocation::DMK_SHADER_LOCATION_FRAGMENT;
	TexSampler.binding = 4;
	descriptor.uniformBufferObjectDescriptions.push_back(TexSampler);

	/* Add the uniform descriptior */
	descriptor.uniformBufferObjectDescriptions.push_back(UBODescriptor);

	UBODescriptor.location = DMKShaderLocation::DMK_SHADER_LOCATION_FRAGMENT;
	UBODescriptor.binding = 0;
	descriptor.uniformBufferObjectDescriptions.push_back(UBODescriptor);

	UBODescriptor.type = DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT;
	UBODescriptor.location = DMKShaderLocation::DMK_SHADER_LOCATION_FRAGMENT;
	UBODescriptor.binding = 1;

	uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_VEC4;
	uAttribute1.arrayCount = 4;
	UBODescriptor.attributes.push_back(uAttribute1);
	uAttribute1.arrayCount = 1;

	uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_F32;
	UBODescriptor.attributes.push_back(uAttribute1);

	uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_F32;
	UBODescriptor.attributes.push_back(uAttribute1);

	descriptor.uniformBufferObjectDescriptions.push_back(UBODescriptor);

	/* Push constants */
	DMKUniformBufferObjectDescriptor PushConstantDescriptor;
	PushConstantDescriptor.type = DMKUniformType::DMK_UNIFORM_TYPE_CONSTANT;
	PushConstantDescriptor.location = DMKShaderLocation::DMK_SHADER_LOCATION_FRAGMENT;

	uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_F32;
	PushConstantDescriptor.attributes.push_back(uAttribute1);

	uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_F32;
	PushConstantDescriptor.attributes.push_back(uAttribute1);

	uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_F32;
	PushConstantDescriptor.attributes.push_back(uAttribute1);

	uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_F32;
	PushConstantDescriptor.attributes.push_back(uAttribute1);

	uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_F32;
	PushConstantDescriptor.attributes.push_back(uAttribute1);

	uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_F32;
	PushConstantDescriptor.attributes.push_back(uAttribute1);

	descriptor.uniformBufferObjectDescriptions.push_back(PushConstantDescriptor);

	PushConstantDescriptor.location = DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX;
	PushConstantDescriptor.attributes.clear();

	uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
	PushConstantDescriptor.attributes.push_back(uAttribute1);

	descriptor.uniformBufferObjectDescriptions.push_back(PushConstantDescriptor);

	/* Add additional attachments */
	descriptor.additionalAttachments.push_back(DMKAttachmentType::DMK_ATTACHMENT_TYPE_IMAGE_BASED_LIGHTING);
}

Dynamik::DMKUniformBufferData Reflect::onUpdate(Dynamik::DMKCameraData data)
{
	uniformBufferDataStore.clear();

	uniformBufferDataStore.addData(&myModelMatrix, sizeof(MAT4), 0);
	uniformBufferDataStore.addData(&data.viewMatrix, sizeof(MAT4), 1);
	uniformBufferDataStore.addData(&data.projectionMatrix, sizeof(MAT4), 2);
	uniformBufferDataStore.addData(&data.cameraPosition, sizeof(VEC3), 3);

	return uniformBufferDataStore;
}