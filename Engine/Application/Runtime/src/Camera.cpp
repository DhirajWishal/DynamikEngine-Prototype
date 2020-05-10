#include "Camera.h"

/* Update camera data function */

Camera::Camera()
{
}

Camera::~Camera()
{
}

DMKCameraData Camera::update(ARRAY<POINTER<DMKEventComponent>> eventComponents)
{
	const F32 movementBias = 0.05f;

	for (auto component : eventComponents)
	{
		if (!component.isValid())
			continue;

		if (component->type == DMKEventType::DMK_EVENT_TYPE_PRESS)
		{
			DMKKeyEventComponent eventContainer = *(DMKKeyEventComponent*)component.get();
			switch (eventContainer.keycode) {
			case DMK_KEY_W:
				myData.cameraPosition += myData.cameraFront * movementBias;
				break;
			case DMK_KEY_A:
				myData.cameraPosition += myData.cameraRight * movementBias;
				break;
			case DMK_KEY_S:
				myData.cameraPosition -= myData.cameraFront * movementBias;
				break;
			case DMK_KEY_D:
				myData.cameraPosition -= myData.cameraRight * movementBias;
				break;
			case DMK_KEY_UP:
				myData.cameraPosition += myData.cameraUp * movementBias;
				break;
			case DMK_KEY_DOWN:
				myData.cameraPosition -= myData.cameraUp * movementBias;
				break;
			case DMK_KEY_LEFT:
				break;
			case DMK_KEY_RIGHT:
				break;
			case DMK_KEY_KP_ADD:
				break;
			case DMK_KEY_KP_SUBTRACT:
				break;
			}
		}
		else if (component->type == DMKEventType::DMK_EVENT_TYPE_REPEAT)
		{
			DMKKeyEventComponent eventContainer = *(DMKKeyEventComponent*)component.get();
			switch (eventContainer.keycode) {
			case DMK_KEY_W:
				myData.cameraPosition += myData.cameraFront * movementBias;
				break;
			case DMK_KEY_A:
				myData.cameraPosition += myData.cameraRight * movementBias;
				break;
			case DMK_KEY_S:
				myData.cameraPosition -= myData.cameraFront * movementBias;
				break;
			case DMK_KEY_D:
				myData.cameraPosition -= myData.cameraRight * movementBias;
				break;
			case DMK_KEY_UP:
				myData.cameraPosition += myData.cameraUp * movementBias;
				break;
			case DMK_KEY_DOWN:
				myData.cameraPosition -= myData.cameraUp * movementBias;
				break;
			case DMK_KEY_LEFT:
				break;
			case DMK_KEY_RIGHT:
				break;
			case DMK_KEY_KP_ADD:
				break;
			case DMK_KEY_KP_SUBTRACT:
				break;
			}
		}
	}

	return myData;
}
