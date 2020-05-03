#pragma once

#ifndef _DYNAMIK_CENTRAL_DATA_HUB_CONTAINERS_H
#define _DYNAMIK_CENTRAL_DATA_HUB_CONTAINERS_H
/*
#include <cstdint>
#include "keyEvent.h"
#include "mouseEvent.h"
#include "applicationEvent.h"

namespace Dynamik {
	enum class DMKEventType {
		DMK_EVENT_TYPE_MOUSE_PRESS,
		DMK_EVENT_TYPE_MOUSE_RELEASE,
		DMK_EVENT_TYPE_MOUSE_MOVE,
		DMK_EVENT_TYPE_MOUSE_SCROLL,
		DMK_EVENT_TYPE_MOUSE_MOVED,

		DMK_EVENT_TYPE_KEY_PRESS,
		DMK_EVENT_TYPE_KEY_RELEASE,
		DMK_EVENT_TYPE_KEY_REPEAT,

		DMK_EVENT_TYPE_UNKNOWN
	};

	struct DMKEventSubSet {
	};

	struct DMKEventContainer {
		DMKEventType eventType = DMKEventType::DMK_EVENT_TYPE_UNKNOWN;
		int32_t code = -1;
		UI32 count = 0;

		float xAxis = 0.0f;
		float yAxis = 0.0f;

		float xOffset = 0.0f;
		float yOffset = 0.0f;
	};

	// Update info
	struct DMKUpdateInfo {
		// movement and rotation biases
		float movementBiasUpDown = 0.005f;
		float movementBiasFrontBack = 0.005f;
		float movementBiasLeftRight = 0.005f;
		float rotationBias = 2.5f;

		// movement and rotaion container
		float upDown = 0.0f;
		float frontBack = 0.0f;
		float leftRight = 0.0f;
		float rotationX = 0.0f;
		float rotationY = 0.0f;
		float rotationZ = 0.0f;

		// movement and rotaion locks
		bool movementUpDownLock = false;
		bool movementFrontBackLock = false;
		bool movementLeftRightLock = false;
		bool movementRotationLock = false;
	};
}
*/
#endif
