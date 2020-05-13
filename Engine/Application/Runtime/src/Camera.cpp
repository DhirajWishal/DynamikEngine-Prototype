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

DMKCameraData Camera::update(std::vector<POINTER<DMKEventComponent>> eventComponents)
{
	const F32 movementBias = 0.05f;
	static CursorPosition _pos;
	static VEC3 scale = glm::vec3(1.0f);
	myData.rayDirection = VEC3(0.0f, 0.0f, -1.0f);

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
				myData.cameraPosition -= myData.cameraRight * movementBias;
				break;
			case DMK_KEY_S:
				myData.cameraPosition -= myData.cameraFront * movementBias;
				break;
			case DMK_KEY_D:
				myData.cameraPosition += myData.cameraRight * movementBias;
				break;
			case DMK_KEY_UP:
				myData.cameraPosition -= myData.cameraUp * movementBias;
				break;
			case DMK_KEY_DOWN:
				myData.cameraPosition += myData.cameraUp * movementBias;
				break;
			case DMK_KEY_LEFT:
				scale -= glm::vec3(1.0f);
				break;
			case DMK_KEY_RIGHT:
				scale += glm::vec3(1.0f);
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
				myData.cameraPosition -= myData.cameraRight * movementBias;
				break;
			case DMK_KEY_S:
				myData.cameraPosition -= myData.cameraFront * movementBias;
				break;
			case DMK_KEY_D:
				myData.cameraPosition += myData.cameraRight * movementBias;
				break;
			case DMK_KEY_UP:
				myData.cameraPosition -= myData.cameraUp * movementBias;
				break;
			case DMK_KEY_DOWN:
				myData.cameraPosition += myData.cameraUp * movementBias;
				break;
			case DMK_KEY_LEFT:
				scale -= glm::vec3(1.0f);
				break;
			case DMK_KEY_RIGHT:
				scale += glm::vec3(1.0f);
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
			if (myData.fieldOfView >= 1.0f && myData.fieldOfView <= 45.0f)
				myData.fieldOfView -= eventContainer.yOffset;
			else if (myData.fieldOfView <= 1.0f)
				myData.fieldOfView = 1.0f;
			else if (myData.fieldOfView >= 45.0f)
				myData.fieldOfView = 45.0f;
		}

	}
	if (!Dynamik::DMKEventManager::isCursorOnCurrent())
	{
		calculateVectors();
		return myData;
	}

	if (DMKEventManager::getMouseButton(DMK_MOUSE_BUTTON_LEFT) == DMK_PRESS)
	{
		_pos = Dynamik::DMKEventManager::getCursorPosition();
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
		calculateRay(_pos);
	}
	else if (DMKEventManager::getMouseButton(DMK_MOUSE_BUTTON_LEFT) == DMK_BUTTON_RELEASE)
	{
		firstMouse = true;
	}

	calculateVectors();

	if (DMKEventManager::getKey(DMK_KEY_LEFT_SHIFT) == DMK_PRESS)
	{
		myData.modelMatrix = glm::translate(glm::mat4(1.0f), myData.cameraPosition + myData.cameraFront);
		myData.modelMatrix *= glm::scale(glm::mat4(1.0f), scale);
	}

	return myData;
}

void Camera::calculateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	myData.cameraFront = glm::normalize(front);
	myData.cameraRight = glm::normalize(glm::cross(myData.cameraFront, worldUp));
	myData.cameraUp = glm::normalize(glm::cross(myData.cameraRight, myData.cameraFront));

	myData.viewMatrix = glm::lookAt(myData.cameraPosition, myData.cameraPosition + myData.cameraFront, myData.cameraUp);
	myData.projectionMatrix = glm::perspective(glm::radians(myData.fieldOfView), myData.aspectRatio, myData.cameraNear, myData.cameraFar);
	myData.projectionMatrix[1][1] *= -1.0f;
}

void Camera::calculateRay(Dynamik::CursorPosition position)
{
	F32 _normalizedX = (2.0f * position.xOffset) / windowWidth - 1.0f;
	F32 _normalizedY = 1.0f - (2.0f * position.yOffset) / windowHeight;
	VEC3 _normalizedVector = VEC3(_normalizedX, _normalizedY, 1.0f);

	VEC4 _clip = VEC4(_normalizedVector.x, _normalizedVector.y, -1.0, 1.0);

	VEC4 _rayOrigin = glm::inverse(myData.projectionMatrix) * _clip;
	_rayOrigin = VEC4(_rayOrigin.x, _rayOrigin.y, -1.0, 0.0);

	myData.rayDirection = VEC3(glm::inverse(myData.viewMatrix) * _rayOrigin);
	myData.rayDirection = glm::normalize(myData.rayDirection);

	std::cout << "\rDirection: " <<
		std::to_string(myData.rayDirection.x) + " " +
		std::to_string(myData.rayDirection.y) + " " +
		std::to_string(myData.rayDirection.z) + " Origin: " +
		std::to_string(myData.cameraPosition.x) + " " +
		std::to_string(myData.cameraPosition.y) + " " +
		std::to_string(myData.cameraPosition.z);
}
