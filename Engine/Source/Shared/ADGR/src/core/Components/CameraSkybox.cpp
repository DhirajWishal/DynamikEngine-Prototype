#include "adgrafx.h"
#include "CameraSkybox.h"

namespace Dynamik {
	namespace ADGR {
		UniformBufferObject CameraSkybox::updateCamera(std::deque<DMKEventContainer> container, DMKUpdateInfo updateInfo)
		{
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
						cameraPos += cameraSpeed * cameraFront;
						break;
					case DMK_KEY_A:
						cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
						break;
					case DMK_KEY_S:
						cameraPos -= cameraSpeed * cameraFront;
						break;
					case DMK_KEY_D:
						cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
						break;
					case DMK_KEY_UP:
						cameraPos += cameraSpeed * cameraUp;
						break;
					case DMK_KEY_DOWN:
						cameraPos -= cameraSpeed * cameraUp;
						break;
					case DMK_KEY_LEFT:
						break;
					case DMK_KEY_RIGHT:
						break;
					}
				}
				else if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT)
				{
					switch (eventContainer.code) {
					case DMK_KEY_W:
						cameraPos += cameraSpeed * cameraFront;
						break;
					case DMK_KEY_A:
						cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
						break;
					case DMK_KEY_S:
						cameraPos -= cameraSpeed * cameraFront;
						break;
					case DMK_KEY_D:
						cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
						break;
					case DMK_KEY_UP:
						cameraPos += cameraSpeed * cameraUp;
						break;
					case DMK_KEY_DOWN:
						cameraPos -= cameraSpeed * cameraUp;
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

			UniformBufferObject uboVS;
			glm::mat4 viewMatrix = glm::mat4(1.0f);
			uboVS.model = glm::mat4(1.0f);
			uboVS.view = glm::mat4(glm::mat3(glCam.GetViewMatrix()));
			uboVS.proj = glm::perspective(glm::radians(45.0f), updateInfo.aspectRatio, 0.1f, 256.0f);
			uboVS.proj[1][1] *= -1;

			return uboVS;
		}
	}
}