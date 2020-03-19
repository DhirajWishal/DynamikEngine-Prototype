#include "objectDefinitions.h"
#include <future>
#include <functional>

void moon::update(Event& currentEvent) {
}

mars::mars() {
	init();
	myUpdateProperties.movementBiasFrontBack = 0.05f;
	myUpdateProperties.movementBiasLeftRight = 0.05f;
	myUpdateProperties.movementBiasUpDown = 0.05f;
}

DMKUpdateInfo mars::draw(std::deque<DMKEventContainer>& eventContainers) {
	{
		Dynamik::ARRAY<std::future<void>, DMKArrayDestructorCallMode::DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_ALL> threads;

		for (int i = 0; i < eventContainers.size(); i++) {
			DMKEventContainer eventContainer = eventContainers.back();
			eventContainers.pop_back();

			if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_PRESS)
				threads.push_back(std::async(std::launch::async, [](DMKEventContainer eventContainer, DMKUpdateInfo* myUpdateProperties) {
				switch (eventContainer.code) {
				case DMK_KEY_W:
					myUpdateProperties->frontBack += myUpdateProperties->movementBiasFrontBack;
					break;
				case DMK_KEY_A:
					myUpdateProperties->leftRight -= myUpdateProperties->movementBiasLeftRight;
					break;
				case DMK_KEY_S:
					myUpdateProperties->frontBack -= myUpdateProperties->movementBiasFrontBack;
					break;
				case DMK_KEY_D:
					myUpdateProperties->leftRight += myUpdateProperties->movementBiasLeftRight;
					break;
				case DMK_KEY_UP:
					myUpdateProperties->upDown -= myUpdateProperties->movementBiasUpDown;
					break;
				case DMK_KEY_DOWN:
					myUpdateProperties->upDown += myUpdateProperties->movementBiasUpDown;
					break;
				case DMK_KEY_LEFT:
					myUpdateProperties->rotationZ -= myUpdateProperties->rotationBias;
					break;
				case DMK_KEY_RIGHT:
					myUpdateProperties->rotationZ += myUpdateProperties->rotationBias;
					break;
				}
					}, eventContainer, &myUpdateProperties));
			else if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT)
				threads.push_back(std::async(std::launch::async, [](DMKEventContainer eventContainer, DMKUpdateInfo* myUpdateProperties) {
				switch (eventContainer.code) {
				case DMK_KEY_W:
					myUpdateProperties->frontBack += myUpdateProperties->movementBiasFrontBack;
					break;
				case DMK_KEY_A:
					myUpdateProperties->leftRight -= myUpdateProperties->movementBiasLeftRight;
					break;
				case DMK_KEY_S:
					myUpdateProperties->frontBack -= myUpdateProperties->movementBiasFrontBack;
					break;
				case DMK_KEY_D:
					myUpdateProperties->leftRight += myUpdateProperties->movementBiasLeftRight;
					break;
				case DMK_KEY_UP:
					myUpdateProperties->upDown -= myUpdateProperties->movementBiasUpDown;
					break;
				case DMK_KEY_DOWN:
					myUpdateProperties->upDown += myUpdateProperties->movementBiasUpDown;
					break;
				case DMK_KEY_LEFT:
					myUpdateProperties->rotationZ -= myUpdateProperties->rotationBias;
					break;
				case DMK_KEY_RIGHT:
					myUpdateProperties->rotationZ += myUpdateProperties->rotationBias;
					break;
				}
					}, eventContainer, &myUpdateProperties));
			else if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_MOUSE_MOVED)
				threads.push_back(std::async(std::launch::async, [](DMKEventContainer eventContainer, DMKUpdateInfo* myUpdateProperties) {
				myUpdateProperties->rotationX = eventContainer.xAxis;
				myUpdateProperties->rotationY = eventContainer.yAxis;
					}, eventContainer, &myUpdateProperties));
		}
	}

	return myUpdateProperties;
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