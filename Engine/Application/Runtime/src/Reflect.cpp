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
	descriptor.vertexBufferObjectDescription.attributes.pushBack(vAttribute1);

	DMKVertexAttribute vAttribute2;		/* location = 1 */
	vAttribute2.name = DMKVertexData::DMK_VERTEX_DATA_COLOR;
	vAttribute2.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
	descriptor.vertexBufferObjectDescription.attributes.pushBack(vAttribute2);

	DMKVertexAttribute vAttribute3;		/* location = 2 */
	vAttribute3.name = DMKVertexData::DMK_VERTEX_DATA_TEXTURE_COORDINATES;
	vAttribute3.dataType = DMKDataType::DMK_DATA_TYPE_VEC2;
	descriptor.vertexBufferObjectDescription.attributes.pushBack(vAttribute3);

	DMKVertexAttribute vAttribute4;		/* location = 3 */
	vAttribute4.name = DMKVertexData::DMK_VERTEX_DATA_INTEGRITY;
	vAttribute4.dataType = DMKDataType::DMK_DATA_TYPE_F32;
	descriptor.vertexBufferObjectDescription.attributes.pushBack(vAttribute4);

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
	UBODescriptor.attributes.pushBack(uAttribute1);

	DMKUniformAttribute uAttribute2;
	uAttribute2.name = DMKUniformData::DMK_UNIFORM_DATA_VIEW;
	uAttribute2.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
	UBODescriptor.attributes.pushBack(uAttribute2);

	DMKUniformAttribute uAttribute3;
	uAttribute3.name = DMKUniformData::DMK_UNIFORM_DATA_PROJECTION;
	uAttribute3.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
	UBODescriptor.attributes.pushBack(uAttribute3);

	DMKUniformAttribute uAttribute4;
	uAttribute4.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute4.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
	UBODescriptor.attributes.pushBack(uAttribute4);

	/* Initialize the Uniform Buffer Data Store */
	uniformBufferDataStore.initialize(UBODescriptor.attributes);

	/* Texture sampler */
	DMKUniformBufferObjectDescriptor TexSampler;
	TexSampler.type = DMKUniformType::DMK_UNIFORM_TYPE_IMAGE_SAMPLER_2D;
	TexSampler.location = DMKShaderLocation::DMK_SHADER_LOCATION_FRAGMENT;
	TexSampler.binding = 1;

	/* Add the uniform descriptior */
	descriptor.uniformBufferObjectDescriptions.pushBack(UBODescriptor);
	descriptor.uniformBufferObjectDescriptions.pushBack(TexSampler);

	/* Add additional attachments */
	descriptor.additionalAttachments.pushBack(DMKAttachmentType::DMK_ATTACHMENT_TYPE_CUBE_MAP);
}

Dynamik::DMKUniformBufferData Reflect::onUpdate(Dynamik::DMKCameraData data)
{
	uniformBufferDataStore.clear();

	MAT4 model = glm::mat4(1.0f);
	uniformBufferDataStore.addData(&model, sizeof(MAT4), 0);
	
	MAT4 view = glm::lookAt(data.cameraPosition, data.cameraPosition + data.cameraFront, data.cameraUp);
	uniformBufferDataStore.addData(&view, sizeof(MAT4), 1);
	
	MAT4 projection = glm::perspective(glm::radians(data.fieldOfView), data.aspectRatio, data.cameraNear, data.cameraFar);
	//projection[1][1] *= -1;
	uniformBufferDataStore.addData(&projection, sizeof(MAT4), 2);

	uniformBufferDataStore.addData(&data.cameraPosition, sizeof(VEC3), 3);

	return uniformBufferDataStore;
}
