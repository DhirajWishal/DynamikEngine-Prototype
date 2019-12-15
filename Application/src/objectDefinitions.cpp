#include "objectDefinitions.h"

void moon::update(Event& currentEvent) {
}

DMKUpdateInfo mars::draw(std::deque<DMKEventContainer>& eventContainers) {
	for (int i = 0; i < eventContainers.size(); i++) {
		DMKEventContainer eventContainer = eventContainers.back();
		eventContainers.pop_back();

		if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_PRESS) {
			switch (eventContainer.code) {
			case DMK_KEY_W:
				myUpdateProperties.frontBack += myUpdateProperties.movementBiasFrontBack;
				break;
			case DMK_KEY_A:
				myUpdateProperties.leftRight -= myUpdateProperties.movementBiasLeftRight;
				break;
			case DMK_KEY_S:
				myUpdateProperties.frontBack -= myUpdateProperties.movementBiasFrontBack;
				break;
			case DMK_KEY_D:
				myUpdateProperties.leftRight += myUpdateProperties.movementBiasLeftRight;
				break;
			case DMK_KEY_UP:
				myUpdateProperties.upDown -= myUpdateProperties.movementBiasUpDown;
				break;
			case DMK_KEY_DOWN:
				myUpdateProperties.upDown += myUpdateProperties.movementBiasUpDown;
				break;
			case DMK_KEY_LEFT:
				myUpdateProperties.rotationZ -= myUpdateProperties.rotationBias;
				break;
			case DMK_KEY_RIGHT:
				myUpdateProperties.rotationZ += myUpdateProperties.rotationBias;
				break;
			}
		}
		if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT) {
			switch (eventContainer.code) {
			case DMK_KEY_W:
				myUpdateProperties.frontBack += myUpdateProperties.movementBiasFrontBack;
				break;
			case DMK_KEY_A:
				myUpdateProperties.leftRight -= myUpdateProperties.movementBiasLeftRight;
				break;
			case DMK_KEY_S:
				myUpdateProperties.frontBack -= myUpdateProperties.movementBiasFrontBack;
				break;
			case DMK_KEY_D:
				myUpdateProperties.leftRight += myUpdateProperties.movementBiasLeftRight;
				break;
			case DMK_KEY_UP:
				myUpdateProperties.upDown -= myUpdateProperties.movementBiasUpDown;
				break;
			case DMK_KEY_DOWN:
				myUpdateProperties.upDown += myUpdateProperties.movementBiasUpDown;
				break;
			case DMK_KEY_LEFT:
				myUpdateProperties.rotationZ -= myUpdateProperties.rotationBias;
				break;
			case DMK_KEY_RIGHT:
				myUpdateProperties.rotationZ += myUpdateProperties.rotationBias;
				break;
			}
		}
		if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_MOUSE_MOVED) {
			myUpdateProperties.rotationX = eventContainer.xAxis;
			myUpdateProperties.rotationY = eventContainer.yAxis;
		}
	}

	return myUpdateProperties;
}

bool vertexDataContainer::vertex::operator==(const vertex& other) {
	return Position == other.Position
		&& Color == other.Color
		&& Normals == other.Normals
		&& TexCoordinates == other.TexCoordinates;
}

std::vector<DMKVertexInputBindingDescription> vertexDataContainer::getVertexInputBindingDescription(int bindCount) {
	std::vector<DMKVertexInputBindingDescription> descriptions = {};

	for (int i = 0; i < bindCount; i++) {
		DMKVertexInputBindingDescription description = {};
		description.binding = i;
		description.stride = sizeof(vertex);
		descriptions.push_back(description);
	}

	return descriptions;
}

std::vector<DMKVertexInputAttributeDescription> vertexDataContainer::getVertexInputAttributeDescription() {
	std::vector<DMKVertexInputAttributeDescription> descriptions = {};

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
