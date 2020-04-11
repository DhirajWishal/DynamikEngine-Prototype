#include "adgrafx.h"
#include "AnimationCamera.h"

namespace Dynamik {
	namespace ADGR {
		UBO_SKELETAL AnimationCamera::updateCamera(std::deque<DMKEventContainer> container, DMKUpdateInfo updateInfo)
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			float angelX = updateInfo.rotationX;
			float angelY = updateInfo.rotationY;
			float angelZ = updateInfo.rotationZ;

			const float cameraSpeed = 0.05f; // adjust accordingly

			for (int i = 0; i < container.size(); i++) {
				DMKEventContainer eventContainer = container.back();
				container.pop_back();

				if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_PRESS)
				{
					switch (eventContainer.code) {
					case DMK_KEY_W:
						glCam.ProcessKeyboard(FORWARD, deltaTime);
						break;
					case DMK_KEY_A:
						glCam.ProcessKeyboard(LEFT, deltaTime);
						break;
					case DMK_KEY_S:
						glCam.ProcessKeyboard(BACKWARD, deltaTime);
						break;
					case DMK_KEY_D:
						glCam.ProcessKeyboard(RIGHT, deltaTime);
						break;
					case DMK_KEY_UP:
						glCam.Position += cameraSpeed * glCam.Up;
						break;
					case DMK_KEY_DOWN:
						glCam.Position -= cameraSpeed * glCam.Up;
						break;
					case DMK_KEY_LEFT:
						break;
					case DMK_KEY_RIGHT:
						break;
					case DMK_KEY_KP_ADD:
						glCam.MovementSpeed += 0.5f;
						break;
					case DMK_KEY_KP_SUBTRACT:
						glCam.MovementSpeed -= 0.5f;
						break;
					}
				}
				else if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT)
				{
					switch (eventContainer.code) {
					case DMK_KEY_W:
						glCam.ProcessKeyboard(FORWARD, deltaTime);
						break;
					case DMK_KEY_A:
						glCam.ProcessKeyboard(LEFT, deltaTime);
						break;
					case DMK_KEY_S:
						glCam.ProcessKeyboard(BACKWARD, deltaTime);
						break;
					case DMK_KEY_D:
						glCam.ProcessKeyboard(RIGHT, deltaTime);
						break;
					case DMK_KEY_UP:
						glCam.Position += cameraSpeed * glCam.Up;
						break;
					case DMK_KEY_DOWN:
						glCam.Position -= cameraSpeed * glCam.Up;
						break;
					case DMK_KEY_LEFT:
						break;
					case DMK_KEY_RIGHT:
						break;
					}
				}
				else if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_MOUSE_MOVED)
				{
					_rotX = eventContainer.xAxis;
					_rotY = eventContainer.yAxis;
				}
				else if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_MOUSE_SCROLL)
				{
					if (zoom >= 1.0f && zoom <= 45.0f)
						zoom -= eventContainer.yOffset;;
					if (zoom <= 1.0f)
						zoom = 1.0f;
					if (zoom >= 45.0f)
						zoom = 45.0f;
				}
			}

			if (updateInfo.useRadians)
			{
				updateInfo.fieldOfView = glm::radians(updateInfo.fieldOfView);
				angelX = cos(glm::radians(_rotX)) * cos(glm::radians(_rotY));
				angelY = sin(glm::radians(_rotY));
				angelZ = sin(glm::radians(_rotX)) * cos(glm::radians(_rotY));
			}

			if (firstMouse)
			{
				lastX = _rotX;
				lastY = _rotY;
				firstMouse = false;
			}

			float xoffset = _rotX - lastX;
			float yoffset = lastY - _rotY; // reversed since y-coordinates go from bottom to top

			lastX = _rotX;
			lastY = _rotY;

			glCam.ProcessMouseMovement(xoffset, yoffset);

			const glm::vec3 scale = glm::vec3(0.0025f);

			UBO_SKELETAL uboVS;
			uboVS.projection = glm::perspective(glm::radians(60.0f), updateInfo.aspectRatio, updateInfo.near, updateInfo.far);
			uboVS.view = glCam.GetViewMatrix();
			uboVS.viewPos = glm::vec4(glCam.Position, 0.0f) * glm::vec4(-1.0f);
			uboVS.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			uboVS.model = glm::scale(uboVS.model, scale);

			return uboVS;
		}
	}
}