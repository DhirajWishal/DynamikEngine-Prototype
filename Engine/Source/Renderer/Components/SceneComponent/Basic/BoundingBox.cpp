#include "dmkafx.h"
#include "BoundingBox.h"

namespace Dynamik {
	void BoundingBox::initialize()
	{
		/* Vertex attributes description */
		DMKVertexAttribute vAttribute1;		/* location = 0 */
		vAttribute1.name = DMKVertexData::DMK_VERTEX_DATA_POSITION;
		vAttribute1.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
		descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute1);

		DMKVertexAttribute vAttribute2;		/* location = 1 */
		vAttribute2.name = DMKVertexData::DMK_VERTEX_DATA_COLOR;
		vAttribute2.dataType = DMKDataType::DMK_DATA_TYPE_VEC3;
		descriptor.vertexBufferObjectDescription.attributes.push_back(vAttribute2);

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

		/* Add the uniform descriptior */
		descriptor.uniformBufferObjectDescriptions.push_back(UBODescriptor);
	}
	
	DMKUniformBufferData BoundingBox::onUpdate(DMKCameraData data)
	{
		uniformBufferDataStore.clear();

		MAT4 model = glm::translate(MAT4(1.0f), descriptor.transformDescriptor.location);
		uniformBufferDataStore.addData(&model, sizeof(MAT4), 0);
		uniformBufferDataStore.addData(&data.viewMatrix, sizeof(MAT4), 1);
		uniformBufferDataStore.addData(&data.projectionMatrix, sizeof(MAT4), 2);

		return uniformBufferDataStore;
	}
}