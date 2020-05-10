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
	const F32 movementBias = 1.0f;

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
		//else if (component->category == DMKEventCategory::DMK_EVENT_CATEGORY_MOUSE_BUTTON)
		//{
		//	auto eventContainer = DMKUtilities::getMouseButtonEvent(component);
		//	if (eventContainer.button == DMK_MOUSE_BUTTON_LEFT)
		//	{
		//		
		//	}
		//}

	}
	if (!Dynamik::DMKEventManager::isCursorOnCurrent());

	auto _pos = Dynamik::DMKEventManager::getCursorPosition();
	angelX = cos(glm::radians(_pos.xOffset)) * cos(glm::radians(_pos.yOffset));
	angelY = sin(glm::radians(_pos.yOffset));
	angelZ = sin(glm::radians(_pos.xOffset)) * cos(glm::radians(_pos.yOffset));

	if (firstMouse)
	{
		lastX = angelX;
		lastY = angelY;
		firstMouse = false;
	}

	float xoffset = angelX - lastX;
	float yoffset = lastY - angelY; // reversed since y-coordinates go from bottom to top

	lastX = angelX;
	lastY = angelY;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	myData.cameraFront = glm::normalize(front);
	myData.cameraRight = glm::normalize(glm::cross(myData.cameraFront, myData.cameraUp));
	myData.cameraUp = glm::normalize(glm::cross(myData.cameraRight, myData.cameraFront));

	return myData;
}
