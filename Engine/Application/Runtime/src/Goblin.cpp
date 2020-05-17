#include "Goblin.h"

void Goblin::initialize()
{
	/* Basic description */
	type = DMKObjectType::DMK_OBJECT_TYPE_SKELETAL_ANIMATION;

	/* Asset description */
	descriptor.assetDescription.dynamikResouceFilePath = "E:/Projects/Dynamik Engine/Game Repository/assets/assets/Skeletal Animation";
	descriptor.assetDescription.physicallyBased = false;
	descriptor.assetDescription.textureInputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_KTX;
	descriptor.assetDescription.textureType = DMKTextureType::DMK_TEXTURE_TYPE_2D;

	/* Transformation description */
	descriptor.transformDescriptor.location = VEC3(3.0f, 0.0f, 4.0f);
	descriptor.transformDescriptor.hitBoxRadius = 1.0f;

	/* Vertex attributes description */
	DMKVertexAttribute vAttribute1;		/* location = 0 */
	vAttribute1.name = DMKVertexData::DMK_VERTEX_DATA_POSITION;
	vAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
	descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute1);

	DMKVertexAttribute vAttribute2;		/* location = 1 */
	vAttribute2.name = DMKVertexData::DMK_VERTEX_DATA_NORMAL_VECTORS;
	vAttribute2.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
	descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute2);

	DMKVertexAttribute vAttribute3;		/* location = 2 */
	vAttribute3.name = DMKVertexData::DMK_VERTEX_DATA_UV_COORDINATES;
	vAttribute3.dataType = DMKDataType::DMK_DATA_TYPE_VEC2;
	descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute3);

	DMKVertexAttribute vAttribute4;		/* location = 3 */
	vAttribute4.name = DMKVertexData::DMK_VERTEX_DATA_COLOR;
	vAttribute4.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
	descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute4);

	DMKVertexAttribute vAttribute5;		/* location = 4 */
	vAttribute5.name = DMKVertexData::DMK_VERTEX_DATA_BONE_WEIGHTS;
	vAttribute5.dataType = DMKDataType::DMK_DATA_TYPE_VEC4;
	descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute5);

	DMKVertexAttribute vAttribute6;		/* location = 5 */
	vAttribute6.name = DMKVertexData::DMK_VERTEX_DATA_BONE_ID;
	vAttribute6.dataType = DMKDataType::DMK_DATA_TYPE_IVEC4;
	descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute6);

	/* Index buffer description */
	descriptor.indexBufferType = DMKDataType::DMK_DATA_TYPE_UI32;

	/* Uniform attributes description */
	DMKUniformBufferObjectDescriptor UBODescriptor;
	UBODescriptor.type = DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT;
	UBODescriptor.location = DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX;
	UBODescriptor.binding = 0;

	DMKUniformAttribute uAttribute1;
	uAttribute1.name = DMKUniformData::DMK_UNIFORM_DATA_PROJECTION;
	uAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
	UBODescriptor.attributes.push_back(uAttribute1);

	DMKUniformAttribute uAttribute2;
	uAttribute2.name = DMKUniformData::DMK_UNIFORM_DATA_VIEW;
	uAttribute2.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
	UBODescriptor.attributes.push_back(uAttribute2);

	DMKUniformAttribute uAttribute3;
	uAttribute3.name = DMKUniformData::DMK_UNIFORM_DATA_MODEL;
	uAttribute3.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
	UBODescriptor.attributes.push_back(uAttribute3);

	DMKUniformAttribute uAttribute4;
	uAttribute4.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute4.dataType = DMKDataType::DMK_DATA_TYPE_MAT4;
	uAttribute4.arrayCount = 64;
	UBODescriptor.attributes.push_back(uAttribute4);

	DMKUniformAttribute uAttribute5;
	uAttribute5.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute5.dataType = DMKDataType::DMK_DATA_TYPE_VEC4;
	UBODescriptor.attributes.push_back(uAttribute5);

	DMKUniformAttribute uAttribute6;
	uAttribute6.name = DMKUniformData::DMK_UNIFORM_DATA_CUSTOM;
	uAttribute6.dataType = DMKDataType::DMK_DATA_TYPE_VEC4;
	UBODescriptor.attributes.push_back(uAttribute6);

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

DMKUniformBufferData Goblin::onUpdate(DMKCameraData data, std::vector<MAT4> boneTransforms)
{
	uniformBufferDataStore.clear();


	//myModelMatrix = glm::rotate(glm::translate(MAT4(1.0f), descriptor.transformDescriptor.location), glm::radians(-90.0f), VEC3(1.0f, 0.0f, 0.0f));
	uniformBufferDataStore.addData(&data.projectionMatrix, sizeof(MAT4), 0);
	uniformBufferDataStore.addData(&data.viewMatrix, sizeof(MAT4), 1);

	MAT4 myModelMatrix = MAT4(1.0f);
	uniformBufferDataStore.addData(&myModelMatrix, sizeof(MAT4), 2);

	uniformBufferDataStore.addData(boneTransforms.data(), sizeof(MAT4) * boneTransforms.size(), 3);
	auto lightPos = VEC4(1.0f);
	uniformBufferDataStore.addData(&lightPos, sizeof(VEC4), 4);
	uniformBufferDataStore.addData(&data.cameraPosition, sizeof(VEC4), 5);

	return uniformBufferDataStore;
}
