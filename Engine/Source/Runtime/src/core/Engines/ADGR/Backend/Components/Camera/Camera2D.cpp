#include "dmkafx.h"
#include "Camera2D.h"

namespace Dynamik {
	namespace ADGR {
		UniformBufferObject Camera2D::updateCamera(std::deque<DMKEventContainer> container, DMKUpdateInfo updateInfo, B1 viewMatrixLock)
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
				angelX = glm::radians(updateInfo.rotationZ);
				angelY = glm::radians(_rotY);
				angelZ = glm::radians(_rotX);
			}

			glm::mat4 _rotationX = glm::rotate(
				glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
				0.0f,
				glm::vec3(0.0f, 0.0f, 1.0f));

			glm::mat4 _rotationY = glm::rotate(
				glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
				0.0f,
				glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 _rotationZ = glm::rotate(
				glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
				0.0f,
				glm::vec3(1.0f, 0.0f, 0.0f));

			UniformBufferObject ubo = {};
			ubo.model = _rotationX * _rotationY * _rotationZ;
			ubo.view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			ubo.proj = glm::perspective(updateInfo.fieldOfView, updateInfo.aspectRatio, updateInfo.near, updateInfo.far);

			ubo.proj[1][1] *= -1;

			return ubo;
		}
	}
}