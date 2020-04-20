#include "objectDefinitions.h"
#include <future>
#include <functional>

mars::mars() {
	init();
}

void mars::init() {
	myVertexDataContainer = vertexDataContainer();
}

void vertexDataContainer::setVertexData(DMKObjectVertexDataType dataType, Dynamik::ARRAY<float> data) {
	switch (dataType) {
	case Dynamik::DMKObjectVertexDataType::DMK_VERTEX_DATA_POSITION:
		vertexData.Position = { data[0], data[1], data[2] };
		break;
	case Dynamik::DMKObjectVertexDataType::DMK_VERTEX_DATA_COLOR:
		vertexData.Color = { data[0], data[1], data[2] };
		break;
	case Dynamik::DMKObjectVertexDataType::DMK_VERTEX_DATA_REFLECTANCE:
		break;
	case Dynamik::DMKObjectVertexDataType::DMK_VERTEX_DATA_TEXTURE_COORDINATES:
		vertexData.TexCoordinates = { data[0], data[1] };
		break;
	case Dynamik::DMKObjectVertexDataType::DMK_VERTEX_DATA_NORMAL_VECTORS:
		vertexData.Normals = { data[0], data[1], data[2] };
		break;
	case Dynamik::DMKObjectVertexDataType::DMK_VERTEX_DATA_TANGENT_VECTORS:
		break;
	case Dynamik::DMKObjectVertexDataType::DMK_VERTEX_DATA_BLEND_WEIGHTS:
		break;
	case Dynamik::DMKObjectVertexDataType::DMK_VERTEX_DATA_BLEND_SHAPES:
		break;
	case Dynamik::DMKObjectVertexDataType::DMK_VERTEX_DATA_BOND_WEIGHTS:
		break;
	case Dynamik::DMKObjectVertexDataType::DMK_VERTEX_DATA_INTEGRITY:
		break;
	case Dynamik::DMKObjectVertexDataType::DMK_VERTEX_DATA_CUSTOM:
		break;
	case Dynamik::DMKObjectVertexDataType::DMK_VERTEX_DATA_UNKNOWN:
		break;
	default:
		break;
	}
}

Dynamik::ARRAY<DMKVertexInputBindingDescription> vertexDataContainer::getVertexInputBindingDescription(int bindCount) {
	Dynamik::ARRAY<DMKVertexInputBindingDescription> descriptions = {};

	for (int i = 0; i < bindCount; i++) {
		DMKVertexInputBindingDescription description = {};
		description.binding = i;
		description.stride = sizeof(vertex);
		descriptions.push_back(description);
	}

	return descriptions;
}

Dynamik::ARRAY<DMKVertexInputAttributeDescription> vertexDataContainer::getVertexInputAttributeDescription() {
	Dynamik::ARRAY<DMKVertexInputAttributeDescription> descriptions = {};

	descriptions[0].binding = 0;
	descriptions[0].location = 0;
	descriptions[0].format = DMK_FORMAT_R32G32B32_SFLOAT;
	descriptions[0].offset = offsetof(vertex, Position);

	descriptions[1].binding = 0;
	descriptions[1].location = 1;
	descriptions[1].format = DMK_FORMAT_R32G32B32_SFLOAT;
	descriptions[1].offset = offsetof(vertex, Color);

	descriptions[2].binding = 0;
	descriptions[2].location = 2;
	descriptions[2].format = DMK_FORMAT_R32G32_SFLOAT;
	descriptions[2].offset = offsetof(vertex, TexCoordinates);

	descriptions[3].binding = 0;
	descriptions[3].location = 3;
	descriptions[3].format = DMK_FORMAT_R32G32_SFLOAT;
	descriptions[3].offset = offsetof(vertex, Normals);

	return descriptions;
}