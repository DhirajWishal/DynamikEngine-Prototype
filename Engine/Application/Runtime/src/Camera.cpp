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
				myData.cameraPosition += cameraFront * movementBias;
				break;
			case DMK_KEY_A:
				myData.cameraPosition -= cameraRight * movementBias;
				break;
			case DMK_KEY_S:
				myData.cameraPosition -= cameraFront * movementBias;
				break;
			case DMK_KEY_D:
				myData.cameraPosition += cameraRight * movementBias;
				break;
			case DMK_KEY_UP:
				myData.cameraPosition -= cameraUp * movementBias;
				break;
			case DMK_KEY_DOWN:
				myData.cameraPosition += cameraUp * movementBias;
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
				myData.cameraPosition += cameraFront * movementBias;
				break;
			case DMK_KEY_A:
				myData.cameraPosition -= cameraRight * movementBias;
				break;
			case DMK_KEY_S:
				myData.cameraPosition -= cameraFront * movementBias;
				break;
			case DMK_KEY_D:
				myData.cameraPosition += cameraRight * movementBias;
				break;
			case DMK_KEY_UP:
				myData.cameraPosition -= cameraUp * movementBias;
				break;
			case DMK_KEY_DOWN:
				myData.cameraPosition += cameraUp * movementBias;
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
			if (fieldOfView >= 1.0f && fieldOfView <= 45.0f)
				fieldOfView -= eventContainer.yOffset;
			else if (fieldOfView <= 1.0f)
				fieldOfView = 1.0f;
			else if (fieldOfView >= 45.0f)
				fieldOfView = 45.0f;
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

	return myData;
}

void Camera::calculateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	cameraFront = glm::normalize(front);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

	myData.viewMatrix = glm::lookAt(myData.cameraPosition, myData.cameraPosition + cameraFront, cameraUp);
	myData.projectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, cameraNear, cameraFar);
	myData.projectionMatrix[1][1] *= -1.0f;
}

void ScreenPosToWorldRay(
	int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
	int screenWidth, int screenHeight,  // Window size, in pixels
	glm::mat4 ViewMatrix,               // Camera position and orientation
	glm::mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
	glm::vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
	glm::vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
) {
	// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
	glm::vec4 lRayStart_NDC(
		((float)mouseX / (float)screenWidth - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		((float)mouseY / (float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
	);

	glm::vec4 lRayEnd_NDC(
		((float)mouseX / (float)screenWidth - 0.5f) * 2.0f,
		((float)mouseY / (float)screenHeight - 0.5f) * 2.0f,
		0.0,
		1.0f
	);

	// The Projection matrix goes from Camera Space to NDC.
	// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
	glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);

	// The View Matrix goes from World Space to Camera Space.
	// So inverse(ViewMatrix) goes from Camera Space to World Space.
	glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);

	glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
	glm::vec4 lRayStart_world = InverseViewMatrix * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
	glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
	glm::vec4 lRayEnd_world = InverseViewMatrix * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;

	// Faster way (just one inverse)
	//glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
	//glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
	//glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;

	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);

	out_origin = glm::vec3(lRayStart_world);
	out_direction = glm::normalize(lRayDir_world);
}

void Camera::calculateRay(Dynamik::CursorPosition position)
{
	ScreenPosToWorldRay(position.xOffset, position.yOffset,
		windowWidth, windowHeight,
		myData.viewMatrix, myData.projectionMatrix,
		myData.rayOrigin, myData.rayDirection);

	std::cout << "\rDirection: " <<
		std::to_string(myData.rayDirection.x) + " " +
		std::to_string(myData.rayDirection.y) + " " +
		std::to_string(myData.rayDirection.z) + " Origin: " +
		std::to_string(myData.cameraPosition.x) + " " +
		std::to_string(myData.cameraPosition.y) + " " +
		std::to_string(myData.cameraPosition.z);
}