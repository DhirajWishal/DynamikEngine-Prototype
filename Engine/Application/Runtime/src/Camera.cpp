#include "Camera.h"
#include "Dynamik.h"
#include "Events/EventManager.h"
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
				myData.cameraPosition += myData.cameraRight * movementBias;
				break;
			case DMK_KEY_A:
				myData.cameraPosition += myData.cameraFront * movementBias;
				break;
			case DMK_KEY_S:
				myData.cameraPosition -= myData.cameraRight * movementBias;
				break;
			case DMK_KEY_D:
				myData.cameraPosition -= myData.cameraFront * movementBias;
				break;
			case DMK_KEY_UP:
				myData.cameraPosition -= myData.cameraUp * movementBias;
				break;
			case DMK_KEY_DOWN:
				myData.cameraPosition += myData.cameraUp * movementBias;
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
				myData.cameraPosition += myData.cameraRight * movementBias;
				break;
			case DMK_KEY_A:
				myData.cameraPosition += myData.cameraFront * movementBias;
				break;
			case DMK_KEY_S:
				myData.cameraPosition -= myData.cameraRight * movementBias;
				break;
			case DMK_KEY_D:
				myData.cameraPosition -= myData.cameraFront * movementBias;
				break;
			case DMK_KEY_UP:
				myData.cameraPosition -= myData.cameraUp * movementBias;
				break;
			case DMK_KEY_DOWN:
				myData.cameraPosition += myData.cameraUp * movementBias;
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
		else if (component->category == DMKEventCategory::DMK_EVENT_CATEGORY_MOUSE_SCROLL)
		{
			auto eventContainer = DMKUtilities::getMouseScrollEvent(component);
			if (myData.fieldOfView >= 1.0f && myData.fieldOfView <= 60.0f)
				myData.fieldOfView -= eventContainer.yOffset;
			else if (myData.fieldOfView <= 1.0f)
				myData.fieldOfView = 1.0f;
			else if (myData.fieldOfView >= 60.0f)
				myData.fieldOfView = 60.0f;
		}

	}
	if (!Dynamik::DMKEventManager::isCursorOnCurrent())
	{
		calculateVectors();
		return myData;
	}

	auto _pos = Dynamik::DMKEventManager::getCursorPosition();
	if (firstMouse)
	{
		lastX = _pos.xOffset;
		lastY = _pos.yOffset;
		firstMouse = false;
	}

	F32 xoffset = _pos.xOffset - lastX;
	F32 yoffset = lastY - _pos.yOffset; // reversed since y-coordinates go from bottom to top

	F32 sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	lastX = _pos.xOffset;
	lastY = _pos.yOffset;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	calculateVectors();

	return myData;
}

void Camera::calculateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	myData.cameraFront = glm::normalize(front);
	myData.cameraRight = glm::normalize(glm::cross(myData.cameraFront, myData.cameraUp));
	myData.cameraUp = glm::normalize(glm::cross(myData.cameraRight, myData.cameraFront));
}
