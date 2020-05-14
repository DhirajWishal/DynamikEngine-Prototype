#include "Camera.h"
#include "Dynamik.h"
#include "Events/EventManager.h"
/* Update camera data function */

#define M_PI 3.141

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
	float mouseX = position.xOffset / (windowWidth * 0.5f) - 1.0f;
	float mouseY = position.yOffset / (windowHeight * 0.5f) - 1.0f;

	glm::mat4 invVP = glm::inverse(myData.projectionMatrix * myData.viewMatrix);
	glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
	glm::vec4 worldPos = invVP * screenPos;

	myData.rayDirection = glm::normalize(glm::vec3(worldPos));

	std::cout << "\rDirection: " <<
		std::to_string(myData.rayDirection.x) + " " +
		std::to_string(myData.rayDirection.y) + " " +
		std::to_string(myData.rayDirection.z) + " Origin: " +
		std::to_string(myData.cameraPosition.x) + " " +
		std::to_string(myData.cameraPosition.y) + " " +
		std::to_string(myData.cameraPosition.z);
}
