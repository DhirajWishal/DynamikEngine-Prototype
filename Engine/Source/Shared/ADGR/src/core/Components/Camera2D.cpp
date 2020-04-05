#include "adgrafx.h"
#include "Camera2D.h"

namespace Dynamik {
	namespace ADGR {
        UniformBufferObject Camera2D::updateCamera(std::deque<DMKEventContainer> container, DMKUpdateInfo updateInfo)
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
				angelX = glm::radians(_rotX);
				angelY = glm::radians(_rotY);
				angelZ = glm::radians(updateInfo.rotationZ);
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

            /*
            static auto startTime = std::chrono::high_resolution_clock::now();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

            UniformBufferObject ubo = {};
            ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            ubo.proj = glm::perspective(updateInfo.fieldOfView, updateInfo.aspectRatio, updateInfo.near, updateInfo.far);
            ubo.proj[1][1] *= -1;

            return ubo;
            */
        }
	}
}